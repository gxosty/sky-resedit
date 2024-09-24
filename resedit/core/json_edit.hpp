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
			int repeat,
			bool is_injected,
			const std::string& compare_key,
			const std::string& path_to_object_in_asset,
			const std::string& path_to_object_in_file
		);

		~JsonReplaceByKeyEdit() override = default;

		void apply(const AssetData& asset_data) override;
		uint64_t get_modified_size(const AssetData& asset_data) override;

	private:
		void _apply(const AssetData& asset_data);

	private:
		std::string _compare_key;
		std::string _path_to_object_in_asset;
		std::string _path_to_object_in_file;
	};
}