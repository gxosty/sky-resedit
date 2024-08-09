#include "resource_pack.hpp"
#include "exceptions.hpp"
#include "replace_edit.hpp"
#include "json_edit.hpp"

#include <nlohmann/json.hpp>

namespace resedit::core
{
	namespace fs = std::filesystem;

	ResourcePack::ResourcePack(const std::filesystem::path& resource_pack_path) : _path{resource_pack_path}
	{
		fs::path metadata_path = resource_pack_path / RESEDIT_RESOURCE_PACK_METADATA_FILENAME;

		if (!fs::exists(metadata_path))
			throw exceptions::ResourcePackMissingMetadata(resource_pack_path);

		nlohmann::json metadata(metadata_path);

		_name = metadata.value("name", RESEDIT_RESOURCE_PACK_DEFAULT_NAME);
		_author = metadata.value("author", RESEDIT_RESOURCE_PACK_DEFAULT_AUTHOR);
		_sky_version = metadata.value("sky_version", 0);

		if (!metadata.contains("edits")) return;

		for (auto& [idx_str, edit_info] : metadata["edits"].items())
		{
			try {
				std::string asset = edit_info["asset"].get<std::string>();
				std::string file = edit_info["file"].get<std::string>();
				std::string edit_type_str = edit_info["edit_type"];

				fs::path file_path = resource_pack_path / file;

				if (edit_type_str == "replace")
				{
					std::unique_ptr<Edit> edit = std::make_unique<ReplaceEdit>(asset, file_path);
					this->add_edit(std::move(edit));
				}
				else if (edit_type_str == "json_replace_by_key")
				{
					std::string compare_key = edit_info["compare_key"].get<std::string>();
					std::string path_to_object_in_asset = edit_info["path_to_object_in_asset"].get<std::string>();
					std::string path_to_object_in_file = edit_info["path_to_object_in_file"].get<std::string>();

					std::unique_ptr<Edit> edit =
						std::make_unique<JsonReplaceByKeyEdit>(asset, file_path, compare_key, path_to_object_in_asset, path_to_object_in_file);

					this->add_edit(std::move(edit));
				}

			} catch (...) {
				throw exceptions::ResourcePackInvalidEdit(_name, std::stoi(idx_str));
			}
		}
	}

	bool ResourcePack::handle(const AssetData& asset_data)
	{
		bool is_modified = false;

		for (auto& edit : _edits)
		{
			if (edit->matches(asset_data.path))
			{
				edit->apply(asset_data);
				is_modified = true;
			}
		}

		return is_modified;
	}

	void ResourcePack::add_edit(std::unique_ptr<Edit> edit)
	{
		_edits.push_back(std::move(edit));
	}
}