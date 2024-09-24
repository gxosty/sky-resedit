#include "resource_pack.hpp"
#include "exceptions.hpp"
#include "replace_edit.hpp"
#include "json_edit.hpp"

#include <nlohmann/json.hpp>

#include <fstream>

#include <misc/Logger.h>

namespace resedit::core
{
	namespace fs = std::filesystem;

	ResourcePack::ResourcePack(const std::filesystem::path& resource_pack_path) : _path{resource_pack_path}, _enabled{true}
	{
		fs::path metadata_path = resource_pack_path / RESEDIT_RESOURCE_PACK_METADATA_FILENAME;

		if (!fs::exists(metadata_path))
			throw exceptions::ResourcePackMissingMetadata(resource_pack_path);

		std::ifstream metadata_file(metadata_path);

		if (!metadata_file.is_open())
			throw std::runtime_error("Cannot open file: " + metadata_path.string());

		nlohmann::json metadata;
		metadata_file >> metadata;
		metadata_file.close();

		_name = metadata.value("name", RESEDIT_RESOURCE_PACK_DEFAULT_NAME);
		_author = metadata.value("author", RESEDIT_RESOURCE_PACK_DEFAULT_AUTHOR);
		_description = metadata.value("description", RESEDIT_RESOURCE_PACK_DEFAULT_DESCRIPTION);
		_version = metadata.value("version", RESEDIT_RESOURCE_PACK_DEFAULT_VERSION);

		if (metadata.contains("sky_version") && metadata["sky_version"].is_string())
		{
			if (metadata["sky_version"].get<std::string>() != "any")
				throw std::runtime_error("Wrong `sky_version` value in metadata. Should be \"any\" or sky build number");

			_sky_version = 0;
		}
		else
		{
			_sky_version = metadata.value("sky_version", 0);
		}

		if (!metadata.contains("edits")) return;

		for (auto& [idx_str, edit_info] : metadata["edits"].items())
		{
			try {
				std::string asset = edit_info["asset"].get<std::string>();
				std::string file = edit_info["file"].get<std::string>();
				int repeat = edit_info.value("repeat", 1);
				bool is_injected = edit_info.value("is_injected", false);
				std::string edit_type_str = edit_info["edit_type"].get<std::string>();

				fs::path file_path = resource_pack_path / file;

				if (edit_type_str == "replace")
				{
					std::unique_ptr<Edit> edit = std::make_unique<ReplaceEdit>(asset, file_path, repeat, is_injected);
					this->add_edit(std::move(edit));
				}
				else if (edit_type_str == "json_replace_by_key")
				{
					if (is_injected)
					{
						LOGW("[%s:%s] | Cannot inject assets (`is_injected == true`) when `edit_type` field is \"JsonReplaceByKeyEdit\"",
							_name.c_str(), idx_str.c_str());
						continue;
					}

					nlohmann::json& params = edit_info["params"];
					std::string compare_key = params["compare_key"].get<std::string>();
					std::string path_to_object_in_asset = params["path_to_object_in_asset"].get<std::string>();
					std::string path_to_object_in_file = params["path_to_object_in_file"].get<std::string>();

					std::unique_ptr<Edit> edit =
						std::make_unique<JsonReplaceByKeyEdit>(
							asset,
							file_path,
							repeat,
							false,
							compare_key,
							path_to_object_in_asset,
							path_to_object_in_file
						);

					this->add_edit(std::move(edit));
				}

			} catch (...) {
				throw exceptions::ResourcePackInvalidEdit(_name, std::stoi(idx_str));
			}
		}
	}

	void ResourcePack::set_enabled(bool enabled)
	{
		_enabled = enabled;
	}

	bool ResourcePack::any_edit_for_asset(const std::string& asset_path)
	{
		for (auto& edit : _edits)
		{
			if (edit->matches(asset_path))
			{
				return true;
			}
		}

		return false;
	}

	bool ResourcePack::handle(const AssetData& asset_data)
	{
		bool is_modified = false;

		for (auto& edit : _edits)
		{
			if (edit->matches(asset_data.path) && !edit->is_applied())
			{
				edit->apply(asset_data);
				is_modified = true;
			}
		}

		return is_modified;
	}

	uint64_t ResourcePack::get_asset_size(const AssetData& asset_data)
	{
		for (auto& edit : _edits)
		{
			if (edit->matches(asset_data.path) && !edit->is_applied())
			{
				return edit->get_modified_size(asset_data);
			}
		}

		return *asset_data.written_len;
	}


	void ResourcePack::add_edit(std::unique_ptr<Edit> edit)
	{
		_edits.push_back(std::move(edit));
	}
}