#include "resource_pack_manager.hpp"

#include <algorithm>

namespace resedit::core
{
	std::weak_ptr<ResourcePack> ResourcePackManager::create(const std::filesystem::path& resource_pack_path)
	{
		return _packs.emplace_back(std::make_shared<ResourcePack>(resource_pack_path));
	}

	void ResourcePackManager::remove(std::weak_ptr<ResourcePack> resource_pack)
	{
		size_t idx = -1;

		if (auto a_pack = resource_pack.lock())
		{
			for (size_t i = 0; i < _packs.size(); i++)
			{
				if (a_pack == _packs[i])
				{
					idx = i;
					break;
				}
			}
		}

		if (idx != -1)
		{
			_packs.erase(_packs.begin() + idx);
		}
	}

	std::weak_ptr<ResourcePack> ResourcePackManager::get(size_t idx) const
	{
		return _packs.at(idx);
	}

	size_t ResourcePackManager::count() const
	{
		return _packs.size();
	}

	int ResourcePackManager::move(size_t idx, MoveDirection direction)
	{
		if (((direction == MoveDirection::Up) && (idx == (_packs.size() - 1)))
			|| ((direction == MoveDirection::Down) && (idx == 0)))
		{
			return idx;
		}

		size_t new_idx = direction == MoveDirection::Up ? idx - 1 : idx + 1;

		std::shared_ptr<ResourcePack> pack = std::move(_packs[idx]);
		_packs[idx] = std::move(_packs[new_idx]);
		_packs[new_idx] = std::move(pack);

		return new_idx;
	}
}