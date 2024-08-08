#pragma once

#include "common.hpp"

#include <string>
#include <filesystem>

namespace resedit::core
{
	class Edit
	{
	public:
		Edit(const std::string& asset_path, const std::filesystem::path& file_path)
			: _asset_path{asset_path}, _file_path{file_path} {}

		virtual ~Edit() = default;

		bool matches(const std::string& asset_path) const { return asset_path == _asset_path; }

		virtual void apply(const AssetData& asset_data) = 0;

	protected:
		std::string _asset_path;
		std::filesystem::path _file_path;

	};
}