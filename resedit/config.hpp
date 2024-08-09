#pragma once

#include <nlohmann/json.hpp>

#include <filesystem>

#define RESEDIT_CONFIG_VERSION 1

namespace resedit
{
	class Config
	{
	public:
		Config(std::filesystem::path config_path);

		void load();
		void save();

		size_t get_resource_pack_count();
		std::string get_resource_pack_id(size_t idx);
		void add_resource_pack_id(const std::string& pack_id);
		void move_resource_pack_id(size_t idx, size_t new_idx);
		void remove_resource_pack_id(size_t idx);

	private:
		std::filesystem::path _config_path;
		nlohmann::json _config;
	};
}