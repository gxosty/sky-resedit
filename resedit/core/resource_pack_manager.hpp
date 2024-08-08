#pragma once

#include "resource_pack.hpp"

#include <vector>
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

		// Gets the number of resource packs in the manager
		//
		// Returns:
		//     (size_t) The number of resource packs
		size_t count() const { return _packs.size(); }

		// Moves a resource pack within the manager
		//
		// Args:
		//     idx: (size_t) The index of the resource pack to move
		//     direction: (MoveDirection) The direction to move the resource pack (Up or Down)
		//
		// Returns:
		//     (int) The new index of the moved resource pack
		int move(size_t idx, MoveDirection direction);

	private:
		std::vector<std::shared_ptr<ResourcePack>> _packs; // Vector of resource packs
	};
}
