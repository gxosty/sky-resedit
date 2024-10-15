#pragma once

#include "edit.hpp"

#include <string>
#include <filesystem>

namespace resedit::core
{
	class AppendEdit : public Edit
	{
	public:
		AppendEdit(const std::string& asset_path, const std::filesystem::path& file_path, int repeat, bool is_injected);
		~AppendEdit() override = default;

		void apply(const AssetData& asset_data) override;
		uint64_t get_modified_size(const AssetData& asset_data) override;

	private:
		void _apply(const AssetData& asset_data);
	};
}