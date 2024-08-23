#include "core/common.hpp"
#include "resedit.hpp"
#include "config.hpp"
#include "resedit_io.hpp"
#include "utils.hpp"

#include <Cipher/Cipher.h>
#include <Cipher/CipherUtils.h>
#include <misc/Logger.h>
#include <fileselector/fileselector.h>

#include <future>
#include <filesystem>
#include <unistd.h>

namespace resedit
{
	namespace fs = std::filesystem;

	State _state = State::NotInitialized;
	ImportResult _last_import_result = ImportResult::None;
	std::string _last_error_message{""};

	std::function<void(std::weak_ptr<core::ResourcePack>, ImportResult)> _imported_callback = [](std::weak_ptr<core::ResourcePack>, ImportResult){};

	Config _config(io::get_config_path());
	core::ResourcePackManager _resource_pack_manager;
	
	CipherBase* _vfs_readfile_cipher_hook = nullptr;
	std::promise<void> _vfs_readfile_promise;
	std::future<void> _vfs_readfile_future = _vfs_readfile_promise.get_future();

	uint64_t (*_vfs_readfile_orig)(void* this_, char* buffer, uint64_t max_len);
	uint64_t _vfs_readfile_hook(void* this_, char* buffer, uint64_t max_len)
	{
		_vfs_readfile_future.wait();

		std::string asset_name = std::string(*(const char**)this_);
		uint64_t written_len = 0;

		if (!asset_name.starts_with("RE_"))
		{
			written_len = _vfs_readfile_orig(this_, buffer, max_len);
		}
		else
		{
			LOGI("Found injected asset: %s", asset_name.c_str());
		}

		AssetData asset_data{
			asset_name,
			buffer,
			max_len,
			&written_len
		};

		if (_resource_pack_manager.handle_asset(asset_data))
		{
			LOGI("Modified: %s", *(const char**)this_);
		}

		return written_len;
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
		uintptr_t vfs_readfile_addr = CipherUtils::CipherScanPattern("FD 7B BD A9 FC 0B 00 F9 F4 4F 02 A9 FD 03 00 91 FF 03 08 D1 03 14 40 F9 F4 03 02 AA F3 03 00 AA", Flags::ReadAndExecute);

		if (vfs_readfile_addr)
		{
			_vfs_readfile_cipher_hook = (new CipherHook())
				->set_Hook((uintptr_t)_vfs_readfile_hook)
				->set_Callback((uintptr_t)&_vfs_readfile_orig)
				->set_Address(vfs_readfile_addr, false)
				->Fire();
		}

		fs::path base_path = io::get_base_path();

		if (!fs::is_directory(base_path))
			fs::create_directory(base_path);

		_initialize_resource_packs();

		if (!vfs_readfile_addr)
		{
			_set_state(State::HookError);
			return;
		}

		_set_state(State::Initialized);
		_vfs_readfile_promise.set_value();
	}

	void _clear_temp_dir()
	{
		fs::remove_all(io::get_temp_directory_path());
	}

	void _import_resource_pack(int fd)
	{
		_last_import_result = ImportResult::None;
		std::weak_ptr<core::ResourcePack> resource_pack;

		size_t file_size = lseek(fd, 0, SEEK_END);
		lseek(fd, 0, SEEK_SET);
		char buffer[file_size + 1];
		read(fd, buffer, file_size);
		close(fd);

		fs::path temp_dir = io::get_temp_directory_path();
		fs::path temp_file_path = temp_dir / "temp.zip";

		if (!fs::is_directory(temp_dir))
			fs::create_directory(temp_dir);

		FILE* temp_file = fopen(temp_file_path.string().c_str(), "w");
		fwrite(buffer, 1, file_size, temp_file);
		fclose(temp_file);

		try
		{
			// Extract metadata to temporary directory
			utils::extract_file_from_zip(temp_file_path, RESEDIT_RESOURCE_PACK_METADATA_FILENAME, temp_dir);
			fs::path metadata_path = temp_dir / RESEDIT_RESOURCE_PACK_METADATA_FILENAME;

			// Try creating temporary resource pack to see if metadata is correct
			core::ResourcePack temp_resource_pack(temp_dir);

			// Create new ID for resource pack
			std::string pack_id = utils::get_time_in_seconds_as_string();

			// Get resource pack's path
			fs::path resource_pack_path = io::get_resource_pack_path(pack_id);
			fs::create_directories(resource_pack_path);

			utils::extract_zip(temp_file_path, resource_pack_path);

			_config.add_resource_pack(pack_id, true);
			_config.save();
			resource_pack = _resource_pack_manager.create(resource_pack_path);

			_last_import_result = ImportResult::Imported;
		}
		catch (const std::runtime_error& error)
		{
			_last_import_result = ImportResult::NotImported;
			_last_error_message = std::string(error.what());
		}

		_clear_temp_dir();

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