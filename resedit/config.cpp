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
				{"packs", nlohmann::json::array()},
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
		return _config["packs"].size();
	}

	std::string Config::get_resource_pack_id(size_t idx)
	{
		return _config["packs"][idx]["id"].get<std::string>();
	}

	bool Config::is_resource_pack_enabled(size_t idx)
	{
		return _config["packs"][idx]["enabled"].get<bool>();
	}

	void Config::set_resource_pack_enabled(size_t idx, bool enabled)
	{
		_config["packs"][idx]["enabled"] = enabled;
	}

	void Config::add_resource_pack(const std::string& pack_id, bool enabled)
	{
		_config["packs"].push_back({
			{"id", pack_id},
			{"enabled", enabled}
		});
	}

	void Config::move_resource_pack(size_t idx, size_t new_idx)
	{
		// I know there is more simpler way but meh

		auto pack = _config["packs"][idx];
		_config["packs"].erase(_config["packs"].begin() + idx);
		_config["packs"].insert(_config["packs"].begin() + new_idx, pack);
	}

	void Config::remove_resource_pack(size_t idx)
	{
		_config["packs"].erase(_config["packs"].begin() + idx);
	}
}
