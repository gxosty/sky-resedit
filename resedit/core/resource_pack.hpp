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
		// Constructs a new ResourcePack instance
		//
		// Args:
		//     resource_pack_path: (const std::filesystem::path&) The path to the resource pack
		ResourcePack(const std::filesystem::path& resource_pack_path);

		// Gets the path to the resource pack
		//
		// Returns:
		//     (const std::filesystem::path&) The path to the resource pack
		const std::filesystem::path& get_path() const { return _path; }

		// Gets the name of the resource pack
		//
		// Returns:
		//     (const std::string&) The name of the resource pack
		const std::string& get_name() const { return _name; }

		// Gets the author of the resource pack
		//
		// Returns:
		//     (const std::string&) The author of the resource pack
		const std::string& get_author() const { return _author; }

		// Gets the edits count of the resource pack
		//
		// Returns:
		//     (size_t) The edits count of the resource pack
		size_t get_edits_count() const { return _edits.size(); }

		// Gets the Sky version that the resource pack supports
		//
		// Returns:
		//     (unsigned) The Sky version that the resource pack supports
		unsigned get_sky_version() const { return _sky_version; }

		// Handles asset data for the resource pack
		//
		//! It can make changes to the asset buffer if valid edit object was found
		//
		// Args:
		//     asset_data: (const AssetData&) The asset data to handle
		void handle(const AssetData& asset_data);

	private:
		// Adds an edit to the resource pack
		//
		// Args:
		//     edit: (std::unique_ptr<Edit>) The edit to add
		void add_edit(std::unique_ptr<Edit> edit);

	private:
		std::filesystem::path _path; // Path to the resource pack
		std::string _name; // Name of the resource pack
		std::string _author; // Author of the resource pack
		unsigned _sky_version; // Supported Sky version
		std::vector<std::unique_ptr<Edit>> _edits; // Vector of edits for the resource pack
	};
}
