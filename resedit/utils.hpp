#pragma once

#include <string>
#include <filesystem>

namespace resedit::utils
{
	void extract_zip(const std::filesystem::path& file_path, const std::filesystem::path& output_path);

	void extract_file_from_zip(
		const std::filesystem::path& file_path,
		const std::string& file_to_get,
		const std::filesystem::path& output_path
	);

	std::string get_time_in_seconds_as_string();
}