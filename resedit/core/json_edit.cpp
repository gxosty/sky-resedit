#include "json_edit.hpp"

namespace resedit::core
{
	JsonReplaceByKeyEdit::JsonReplaceByKeyEdit(
		const std::string& asset_path,
		const std::filesystem::path& file_path,
		int repeat,
		const std::string& compare_key,
		const std::string& path_to_object_in_asset,
		const std::string& path_to_object_in_file
	) : Edit(asset_path, file_path, repeat),
		_compare_key{compare_key},
		_path_to_object_in_asset{path_to_object_in_asset},
		_path_to_object_in_file{path_to_object_in_file} {}

	void JsonReplaceByKeyEdit::apply(const AssetData& asset_data)
	{
		return;
	}
}