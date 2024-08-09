#pragma once

#include "core/resource_pack_manager.hpp"

#include <functional>
#include <memory>

namespace resedit
{
	enum class State {
		NotInitialized = 0,
		Initialized,
		HookError
	};

	enum class ImportResult {
		None,
		NotImported,
		Imported
	};

	// Gets the current state of ResEdit.
	State get_state();

	// Initializes ResEdit.
	void initialize();

	// Imports a new resource pack from storage.
	void import_resource_pack();

	// Gets result of the last import
	//
	// Returns:
	//     (ImportResult) result of the last import
	ImportResult get_last_import_result();

	// Sets the callback to be called to inform about the result of last import
	//
	// Args:
	//     callback: (std::function<void(std::weak_ptr<core::ResourcePack>, ImportResult)>) callback to set
	void set_import_callback(std::function<void(std::weak_ptr<core::ResourcePack>, ImportResult)> callback);

	// Removes a resource pack.
	//
	// Args:
	//     resource_pack: (std::weak_ptr<core::ResourcePack>) A weak pointer to the resource pack to remove.
	void remove_resource_pack(std::weak_ptr<core::ResourcePack> resource_pack);

	// Moves a resource pack up or down in the priority list.
	//
	// Args:
	//     resource_pack: (std::weak_ptr<core::ResourcePack>) A weak pointer to the resource pack to move.
	//     direction: (core::ResourcePackManager::MoveDirection) The direction to move the resource pack (up or down).
	//
	// Returns:
	//     (bool) `true` if the resource pack was moved successfully, `false` otherwise.
	bool move_resource_pack(std::weak_ptr<core::ResourcePack> resource_pack, core::ResourcePackManager::MoveDirection direction);

	// Gets the number of loaded resource packs.
	//
	// Returns:
	//     (size_t) The number of loaded resource packs.
	size_t get_resource_pack_count();

	// Gets a resource pack at the specified index.
	//
	// Args:
	//     idx: (size_t) The index of the resource pack to get.
	//
	// Returns:
	//     (std::weak_ptr<core::ResourcePack>) A weak pointer to the resource pack at the specified index.
	std::weak_ptr<core::ResourcePack> get_resource_pack(size_t idx);
}
