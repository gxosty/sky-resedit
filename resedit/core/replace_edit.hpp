#pragma once

#include "edit.hpp"

#include <string>
#include <filesystem>

namespace resedit::core
{
	class ReplaceEdit : public Edit
	{
	public:
		ReplaceEdit(const std::string& asset_path, const std::filesystem::path& file_path, int repeat, bool is_injected);
		~ReplaceEdit() override = default;

		void apply(const AssetData& asset_data) override;
		uint64_t get_modified_size(const AssetData& asset_data) override;

	private:
		void _apply(const AssetData& asset_data);
	};
}