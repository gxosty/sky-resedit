#pragma once

#include "common.hpp"
#include "resource_pack.hpp"

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <filesystem>

namespace resedit::core
{
	class ResourcePackManager
	{
	public:
		enum class MoveDirection { Up, Down };

	public:
		// Constructs a new ResourcePackManager instance
		ResourcePackManager() = default;

		// Creates new resource pack in the manager
		//
		// Args:
		//     resource_pack_path: (const std::filesystem::path&) The resource pack path to create new resource pack
		//
		// Returns:
		//     (std::weak_ptr<ResourcePack>) A weak pointer to the created resource pack
		std::weak_ptr<ResourcePack> create(const std::filesystem::path& resource_pack_path);

		// Removes a resource pack from the manager
		//
		// Args:
		//     idx: (size_t) The index of the resource pack
		void remove(size_t idx);

		// Removes a resource pack from the manager
		//
		// Args:
		//     resource_pack: (std::weak_ptr<ResourcePack>) The resource pack to remove
		void remove(std::weak_ptr<ResourcePack> resource_pack);

		// Gets a resource pack by its index
		//
		// Args:
		//     idx: (size_t) The index of the resource pack
		//
		// Returns:
		//     (std::weak_ptr<ResourcePack>) A weak pointer to the resource pack at the specified index
		std::weak_ptr<ResourcePack> get(size_t idx) const { return _packs.at(idx); }

		// Gets a resource pack index by its pointer
		//
		// Args:
		//     resource_pack: (std::weak_ptr<ResourcePack>) The resource pack to get the index of
		//
		// Returns:
		//     (size_t) The index of the resource pack
		size_t get_index(std::weak_ptr<ResourcePack> resource_pack) const;

		// Gets the number of resource packs in the manager
		//
		// Returns:
		//     (size_t) The number of resource packs
		size_t count() const { return _packs.size(); }

		// Moves a resource pack within the manager
		//
		// Args:
		//     resource_pack: (std::weak_ptr<ResourcePack>) The resource pack to move
		//     direction: (MoveDirection) The direction to move the resource pack (Up or Down)
		//
		// Returns:
		//     (size_t) The new index of the moved resource pack
		size_t move(size_t idx, MoveDirection direction);

		// Moves a resource pack within the manager
		//
		// Args:
		//     idx: (size_t) The index of the resource pack to move
		//     direction: (MoveDirection) The direction to move the resource pack (Up or Down)
		//
		// Returns:
		//     (size_t) The new index of the moved resource pack
		size_t move(std::weak_ptr<ResourcePack> resource_pack, MoveDirection direction);

		// Checks if any resource pack can edit the asset
		//
		// Args
		//     asset_path: (const std::string&) The asset path
		//
		// Returns
		//     (bool) `true` if any resource pack can edit the asset, otherwise `false` 
		bool any_edit_for_asset(const std::string& asset_path);

		// Passes asset data to resource packs for editing
		//
		// Args:
		//     asset_data: (const AssetData&) The asset data to be edited
		//
		// Returns:
		//     (bool) `true` if asset was edited/modified, `false` otherwise
		bool edit_asset(const AssetData& asset_data);

		// Gets size of the asset if any resource pack modifies it
		//
		// Args:
		//     asset_data: (const AssetData&) The asset data to get size of
		//
		// Returns:
		//     (uint64_t) The size of the asset if any resource pack modifies it, otherwise `asset_data.written_len`
		uint64_t get_asset_size(const AssetData& asset_data);

	private:
		std::vector<std::shared_ptr<ResourcePack>> _packs; // Vector of resource packs
	};
}
