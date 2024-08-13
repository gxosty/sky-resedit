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
		void add_resource_pack(const std::string& pack_id, bool enabled);
		std::string get_resource_pack_id(size_t idx);
		bool is_resource_pack_enabled(size_t idx);
		void set_resource_pack_enabled(size_t idx, bool enabled);
		void move_resource_pack(size_t idx, size_t new_idx);
		void remove_resource_pack(size_t idx);

	private:
		std::filesystem::path _config_path;
		nlohmann::json _config;
	};
}