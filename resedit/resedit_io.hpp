#pragma once

#include <filesystem>

namespace resedit::io
{
	std::filesystem::path get_base_path();
	std::filesystem::path get_config_path();
	std::filesystem::path get_resource_pack_path(const std::string& pack_id);
	std::filesystem::path get_temp_directory_path();
}