#pragma once

#include "edit.hpp"

#include <string>
#include <filesystem>

namespace resedit::core
{
	class JsonReplaceByKeyEdit : public Edit
	{
	public:
		JsonReplaceByKeyEdit(
			const std::string& asset_path,
			const std::filesystem::path& file_path,
			const std::string& compare_key,
			const std::string& path_to_object_in_asset,
			const std::string& path_to_object_in_file
		);

		~JsonReplaceByKeyEdit() override = default;

		void apply(const AssetData& asset_data) override;

	private:
		std::string _compare_key;
		std::string _path_to_object_in_asset;
		std::string _path_to_object_in_file;
	};
}