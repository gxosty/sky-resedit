#pragma once

#include "edit.hpp"

#include <string>
#include <filesystem>

namespace resedit::core
{
	class ReplaceEdit : public Edit
	{
	public:
		ReplaceEdit(const std::string& asset_path, const std::filesystem::path& file_path);
		~ReplaceEdit() override = default;

		void apply(const AssetData& asset_data) override;
	};
}