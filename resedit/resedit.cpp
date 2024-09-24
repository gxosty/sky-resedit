#include "core/common.hpp"
#include "resedit.hpp"
#include "config.hpp"
#include "resedit_io.hpp"
#include "asset_manager_hook.hpp"
#include "utils.hpp"

#include <Cipher/Cipher.h>
#include <Cipher/CipherUtils.h>
#include <misc/Logger.h>
#include <fileselector/fileselector.h>

#include <thread>
#include <chrono>
#include <filesystem>
#include <cmath>
#include <unistd.h>

namespace resedit
{
	namespace fs = std::filesystem;

	State _state = State::NotInitialized;
	ImportResult _last_import_result = ImportResult::None;
	std::string _last_error_message{""};

	std::function<void(std::weak_ptr<core::ResourcePack>, ImportResult)> _imported_callback;

	Config _config(io::get_config_path());
	core::ResourcePackManager _resource_pack_manager;

	CipherBase* _vfs_readfile_cipher_hook = nullptr;

	uint64_t asset_read(AAsset* asset, const std::string& asset_path, bool is_injected, char* buffer, uint64_t max_len)
	{
		if (_state == State::NotInitialized)
		{
			LOGI("`AAsset_read` called, but resedit hasn't been initialized yet, waiting...");

			while (_state == State::NotInitialized)
			{
				// simple ways are always the best
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
		}

		if (_resource_pack_manager.any_edit_for_asset(asset_path))
		{
			if (!is_injected)
			{
				auto asset_size = asset_manager_hook::aasset_get_length_orig(asset);
				uint64_t written_len = 0;
				uint64_t inner_buffer_size = fmax(asset_size, max_len);
				char* inner_buffer = new char[inner_buffer_size];
				inner_buffer[asset_size-1] = 0;
				written_len = asset_manager_hook::aasset_read_orig(asset, (void*)inner_buffer, inner_buffer_size);

				AssetData asset_data{
					asset_path,
					inner_buffer,
					inner_buffer_size,
					&written_len
				};

				if (_resource_pack_manager.edit_asset(asset_data))
				{
					LOGI("Modified: %s", asset_path.c_str());
					memcpy(buffer, (void*)inner_buffer, written_len);
				}

				delete[] inner_buffer;

				return written_len;
			}
			else
			{
				uint64_t written_len = 0;

				AssetData asset_data{
					asset_path,
					buffer,
					max_len,
					&written_len
				};

				if (_resource_pack_manager.edit_asset(asset_data))
				{
					LOGI("Injected: %s", asset_path.c_str());
				}

				return written_len;
			}
		}

		return asset_manager_hook::aasset_read_orig(asset, buffer, max_len);
	}

	off_t asset_get_length(AAsset* asset, const std::string& asset_path, bool is_injected)
	{
		auto asset_size =
			(!is_injected ? asset_manager_hook::aasset_get_length_orig(asset) : 0);

		if (_resource_pack_manager.any_edit_for_asset(asset_path))
		{
			if (!is_injected)
			{
				uint64_t max_len = pow(2, (ceil(log2(asset_size))));
				char* buffer = new char[max_len];
				buffer[max_len-1] = 0;
				AAsset_seek(asset, 0, SEEK_SET);
				uint64_t written_len = asset_manager_hook::aasset_read_orig(asset, (void*)buffer, max_len);
				AAsset_seek(asset, 0, SEEK_SET);

				AssetData asset_data{
					asset_path,
					buffer,
					max_len,
					&written_len
				};

				asset_size = _resource_pack_manager.get_asset_size(asset_data);

				delete[] buffer;
			}
			else
			{
				uint64_t written_len = 0;
				AssetData asset_data{
					asset_path,
					nullptr,
					0,
					&written_len
				};

				asset_size = _resource_pack_manager.get_asset_size(asset_data);
			}
		}

		return asset_size;
	}

	void _set_state(State state)
	{
		_state = state;
	}

	State get_state()
	{
		return _state;
	}

	void _initialize_resource_packs()
	{
		for (size_t idx = 0; idx < _config.get_resource_pack_count(); idx++)
		{
			std::string pack_id = _config.get_resource_pack_id(idx);
			fs::path resource_pack_path = io::get_resource_pack_path(pack_id);
			auto resource_pack = _resource_pack_manager.create(resource_pack_path);

			if (auto pack = resource_pack.lock())
			{
				pack->set_enabled(_config.is_resource_pack_enabled(idx));
			}
		}
	}

	void initialize()
	{
		asset_manager_hook::set_asset_read_callback(asset_read);
		asset_manager_hook::set_asset_get_length_callback(asset_get_length);
		asset_manager_hook::apply_hooks();

		fs::path base_path = io::get_base_path();

		if (!fs::is_directory(base_path))
			fs::create_directory(base_path);

		_initialize_resource_packs();

		_set_state(State::Initialized);
	}

	void initialize_late()
	{
		asset_manager_hook::restore_hooks();
	}

	void _clear_temp_dir()
	{
		fs::remove_all(io::get_temp_directory_path());
	}

	void _import_resource_pack(int fd)
	{
		LOGI("Importing resource pack");
		_last_import_result = ImportResult::None;
		std::weak_ptr<core::ResourcePack> resource_pack;

		size_t file_size = lseek(fd, 0, SEEK_END);
		LOGI("File size: %lu", file_size);
		lseek(fd, 0, SEEK_SET);
		LOGI("Allocating heap for file");
		// do NOT do stack allocation
		char* buffer = (char*)malloc(file_size);
		LOGI("Reading file");
		read(fd, buffer, file_size);
		LOGI("Closing file handler");
		close(fd);

		fs::path temp_dir = io::get_temp_directory_path();
		fs::path temp_file_path = temp_dir / "temp.zip";

		LOGI("Temp file path: %s", temp_file_path.string().c_str());

		if (!fs::is_directory(temp_dir))
			fs::create_directory(temp_dir);

		LOGI("Copying resource pack to temporary path");
		FILE* temp_file = fopen(temp_file_path.string().c_str(), "w");
		fwrite(buffer, 1, file_size, temp_file);
		fclose(temp_file);
		free(buffer);
		LOGI("Copying completed");

		try
		{
			LOGI("Extracting resource pack metadata");
			// Extract metadata to temporary directory
			utils::extract_file_from_zip(temp_file_path, RESEDIT_RESOURCE_PACK_METADATA_FILENAME, temp_dir);
			fs::path metadata_path = temp_dir / RESEDIT_RESOURCE_PACK_METADATA_FILENAME;

			LOGI("Checking resource pack for validity");

			// Try creating temporary resource pack to see if metadata is correct
			core::ResourcePack temp_resource_pack(temp_dir);

			LOGI("Resource pack valid");

			// Create new ID for resource pack
			std::string pack_id = utils::get_time_in_seconds_as_string();

			LOGI("Assigned ID: %s", pack_id.c_str());

			// Get resource pack's path
			fs::path resource_pack_path = io::get_resource_pack_path(pack_id);
			fs::create_directories(resource_pack_path);

			LOGI("Extracting resource pack");
			utils::extract_zip(temp_file_path, resource_pack_path);

			LOGI("Saving resource pack to config");
			_config.add_resource_pack(pack_id, true);
			_config.save();
			resource_pack = _resource_pack_manager.create(resource_pack_path);

			_last_import_result = ImportResult::Imported;
		}
		catch (const std::runtime_error& error)
		{
			LOGE("Error: %s", error.what());
			_last_import_result = ImportResult::NotImported;
			_last_error_message = std::string(error.what());
		}

		LOGI("Clearing temp dir");
		_clear_temp_dir();

		LOGI("Calling `_imported_callback`");

		if (_imported_callback)
			_imported_callback(resource_pack, _last_import_result);
	}

	void import_resource_pack()
	{
		requestFile("application/zip", _import_resource_pack, false);
	}

	ImportResult get_last_import_result()
	{
		return _last_import_result;
	}

	void set_import_callback(std::function<void(std::weak_ptr<core::ResourcePack>, ImportResult)> callback)
	{
		_imported_callback = callback;
	}

	void remove_resource_pack(std::weak_ptr<core::ResourcePack> resource_pack)
	{
		fs::path resource_pack_path{"/"};

		if (auto rp = resource_pack.lock())
		{
			resource_pack_path = rp->get_path();
		}

		size_t idx = _resource_pack_manager.get_index(resource_pack);
		_resource_pack_manager.remove(resource_pack);
		_config.remove_resource_pack(idx);
		_config.save();

		if (fs::exists(resource_pack_path / RESEDIT_RESOURCE_PACK_METADATA_FILENAME))
			fs::remove_all(resource_pack_path);
	}

	bool move_resource_pack(std::weak_ptr<core::ResourcePack> resource_pack, core::ResourcePackManager::MoveDirection direction)
	{
		size_t idx = _resource_pack_manager.get_index(resource_pack);
		size_t new_idx = _resource_pack_manager.move(idx, direction);

		bool pack_moved = idx != new_idx;

		if (pack_moved)
		{
			_config.move_resource_pack(idx, new_idx);
			_config.save();
		}

		return pack_moved;
	}

	void set_resource_pack_enabled(std::weak_ptr<core::ResourcePack> resource_pack, bool enabled)
	{
		size_t idx = _resource_pack_manager.get_index(resource_pack);

		if (auto pack = resource_pack.lock())
		{
			pack->set_enabled(enabled);
			_config.set_resource_pack_enabled(idx, enabled);
			_config.save();
		}
	}

	size_t get_resource_pack_count()
	{
		return _resource_pack_manager.count();
	}

	std::weak_ptr<core::ResourcePack> get_resource_pack(size_t idx)
	{
		return _resource_pack_manager.get(idx);
	}
}