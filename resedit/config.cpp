#include "config.hpp"

#include <fstream>
#include <stdexcept>

namespace resedit
{
	namespace fs = std::filesystem;

	Config::Config(fs::path config_path) : _config_path{config_path}
	{
		this->load();
	}

	void Config::load()
	{
		if (!fs::exists(_config_path))
		{
			_config = {
				{"pack_ids", std::vector<std::string>{}},
				{"version", RESEDIT_CONFIG_VERSION}
			};

			return;
		}

		std::ifstream file(_config_path);

		if (!file.is_open())
			throw std::runtime_error("Couldn't open config.json");

		file >> _config;
		file.close();
	}

	void Config::save()
	{
		std::ofstream file(_config_path);

		if (!file.is_open())
			throw std::runtime_error("Couldn't open config.json");

		file << std::setw(4) << _config;
		file.close();
	}

	size_t Config::get_resource_pack_count()
	{
		return _config["pack_ids"].size();
	}

	std::string Config::get_resource_pack_id(size_t idx)
	{
		return _config["pack_ids"][idx].get<std::string>();
	}

	void Config::add_resource_pack_id(const std::string& pack_id)
	{
		_config["pack_ids"].push_back(pack_id);
	}

	void Config::move_resource_pack_id(size_t idx, size_t new_idx)
	{
		// I know there is more simpler way but meh

		std::string pack_id = _config["pack_ids"][idx].get<std::string>();
		_config["pack_ids"].erase(_config["pack_ids"].begin() + idx);
		_config["pack_ids"].insert(_config["pack_ids"].begin() + new_idx, pack_id);
	}

	void Config::remove_resource_pack_id(size_t idx)
	{
		_config["pack_idx"].erase(_config["pack_ids"].begin() + idx);
	}
}
