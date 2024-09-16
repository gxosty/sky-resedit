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

		// Gets the description of the resource pack
		//
		// Returns:
		//     (const std::string&) The description of the resource pack
		const std::string& get_description() const { return _description; }

		// Gets the version of the resource pack
		//
		// Returns:
		//     (const std::string&) The version of the resource pack
		const std::string& get_version() const { return _version; }

		// Gets the Sky version that the resource pack supports
		//
		// Returns:
		//     (unsigned) The Sky version that the resource pack supports
		unsigned get_sky_version() const { return _sky_version; }

		// Gets the edits count of the resource pack
		//
		// Returns:
		//     (size_t) The edits count of the resource pack
		size_t get_edits_count() const { return _edits.size(); }

		// Gets the enabled state of the resource pack
		//
		// Returns:
		//     (bool) The enabled state of the resource pack
		bool is_enabled() const { return _enabled; }

		// Sets the enabled state of the resource pack
		//
		// Args:
		//     enabled: (bool) The enabled state to set
		void set_enabled(bool enabled);

		// Checks if the resource pack can edit the asset
		//
		// Args
		//     asset_path: (const std::string&) The asset path
		//
		// Returns
		//     (bool) `true` if the resource pack can edit the asset, otherwise `false` 
		bool any_edit_for_asset(const std::string& asset_path);

		// Handles asset data for the resource pack
		//
		//! It can make changes to the asset buffer if valid edit object was found
		//
		// Args:
		//     asset_data: (const AssetData&) The asset data to handle
		//
		// Returns:
		//     (bool) `true` if asset was edited/modified, `false` otherwise
		bool handle(const AssetData& asset_data);

		uint64_t get_asset_size(const AssetData& asset_data);

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
		std::string _description; // Description of the resource pack
		std::string _version;
		unsigned _sky_version; // Supported Sky version

		bool _enabled;

		std::vector<std::unique_ptr<Edit>> _edits; // Vector of edits for the resource pack
	};
}
