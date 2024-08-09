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

	State get_state();

	void initialize();
	void import_resource_pack();
	void remove_resource_pack(std::weak_ptr<core::ResourcePack> resource_pack);
	bool move_resource_pack(std::weak_ptr<core::ResourcePack> resource_pack, core::ResourcePackManager::MoveDirection direction);
	size_t get_resource_pack_count();
	std::weak_ptr<core::ResourcePack> get_resource_pack(size_t idx);
}