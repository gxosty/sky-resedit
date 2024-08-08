#pragma once

#include "common.hpp"
#include "edit.hpp"

#include <string>
#include <filesystem>
#include <vector>
#include <memory>

namespace resedit::core
{
	class ResourcePack
	{
	public:
		ResourcePack(const std::filesystem::path& resource_pack_path);

		const std::filesystem::path& get_path() const { return _path; }
		const std::string& get_name() const { return _name; }
		const std::string& get_author() const { return _author; }
		unsigned get_sky_version() const { return _sky_version; }

		void add_edit(std::unique_ptr<Edit> edit);
		void handle(const AssetData& asset_data);

	private:
		std::filesystem::path _path;

		std::string _name;
		std::string _author;
		unsigned _sky_version;

		std::vector<std::unique_ptr<Edit>> _edits;
	};
}