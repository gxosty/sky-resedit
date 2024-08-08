#include "resource_pack.hpp"

#include <nlohmann/json.hpp>

namespace resedit::core
{
	ResourcePack::ResourcePack(const std::filesystem::path& resource_pack_path) : _path{resource_pack_path}
	{
		return;
	}

	void ResourcePack::add_edit(std::unique_ptr<Edit> edit)
	{
		this->_edits.push_back(std::move(edit));
	}

	void ResourcePack::handle(const AssetData& asset_data)
	{
		return;
	}
}