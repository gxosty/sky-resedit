#include "resource_pack_manager.hpp"

#include <algorithm>

namespace resedit::core
{
	static std::unique_ptr<Edit> _null_edit(nullptr);

	std::weak_ptr<ResourcePack> ResourcePackManager::create(const std::filesystem::path& resource_pack_path)
	{
		return _packs.emplace_back(std::make_shared<ResourcePack>(resource_pack_path));
	}

	size_t ResourcePackManager::get_index(std::weak_ptr<ResourcePack> resource_pack) const
	{
		if (auto a_pack = resource_pack.lock())
		{
			for (size_t i = 0; i < _packs.size(); i++)
			{
				if (a_pack == _packs[i])
				{
					return i;
				}
			}
		}

		return -1;
	}

	void ResourcePackManager::remove(size_t idx)
	{
		_packs.erase(_packs.begin() + idx);
	}

	void ResourcePackManager::remove(std::weak_ptr<ResourcePack> resource_pack)
	{
		size_t idx = this->get_index(resource_pack);

		if (idx != -1)
		{
			this->remove(idx);
		}
	}

	size_t ResourcePackManager::move(size_t idx, MoveDirection direction)
	{
		if (((direction == MoveDirection::Up) && (idx == 0))
			|| ((direction == MoveDirection::Down) && (idx == (_packs.size() - 1))))
		{
			return idx;
		}

		size_t new_idx = direction == MoveDirection::Up ? idx - 1 : idx + 1;

		std::shared_ptr<ResourcePack> pack = std::move(_packs[idx]);
		_packs[idx] = std::move(_packs[new_idx]);
		_packs[new_idx] = std::move(pack);

		return new_idx;
	}

	size_t ResourcePackManager::move(std::weak_ptr<ResourcePack> resource_pack, MoveDirection direction)
	{
		size_t idx = this->get_index(resource_pack);

		if (idx != -1)
		{
			return this->move(idx, direction);
		}

		return idx;
	}

	bool ResourcePackManager::any_edit_for_asset(const std::string& asset_path)
	{
		for (auto& pack : _packs)
		{
			if (pack->any_edit_for_asset(asset_path))
			{
				return true;
			}
		}

		return false;
	}

	bool ResourcePackManager::edit_asset(const AssetData& asset_data)
	{
		if (_packs.empty()) return false;

		bool is_modified = false;

		for (auto pack_it = _packs.end() - 1; pack_it >= _packs.begin(); pack_it--)
		{
			if ((*pack_it)->is_enabled())
				is_modified = (*pack_it)->handle(asset_data) || is_modified;
		}

		return is_modified;
	}

	uint64_t ResourcePackManager::get_asset_size(const AssetData& asset_data)
	{
		for (auto& pack : _packs)
		{
			if (pack->any_edit_for_asset(asset_data.path))
			{
				return pack->get_asset_size(asset_data);
			}
		}

		return *asset_data.written_len;
	}

	const std::unique_ptr<Edit>& ResourcePackManager::get_asset_edit(const std::string& asset_path)
	{
		if (_packs.empty()) return _null_edit;

		for (auto pack_it = _packs.end() - 1; pack_it >= _packs.begin(); pack_it--)
		{
			for (auto& edit : (*pack_it)->_edits)
			{
				if (edit->matches(asset_path))
				{
					return edit;
				}
			}
		}

		return _null_edit;
	}
}