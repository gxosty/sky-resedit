#include "utils.hpp"

#include <minizip.c>

#include <ctime>
#include <sstream>

namespace resedit::utils
{
	namespace fs = std::filesystem;

	void extract_zip(const fs::path& file_path, const fs::path& output_path)
	{
		extract_file_from_zip(file_path, "", output_path);
	};

	void extract_file_from_zip(const fs::path& file_path, const std::string& file_to_get, const fs::path& output_path)
	{
		int32_t err;

		minizip_opt mz_options;
		memset(&mz_options, 0, sizeof(mz_options));
		mz_options.overwrite = 1;

		err = minizip_extract(file_path.string().c_str(), (file_to_get.empty() ? NULL : file_to_get.c_str()), output_path.string().c_str(), NULL, &mz_options);

		if (err != MZ_OK) {
			if (file_to_get.empty()) {
				throw std::runtime_error("Error when extracting '" + file_path.string() + "'");
			} else {
				throw std::runtime_error("Error when extracting '" + file_to_get + "' from '" + file_path.string() + "'");
			}
		}
	}

	std::string get_time_in_seconds_as_string() {
		// Get the current time in seconds since the Unix epoch
		time_t now = time(nullptr);

		// Convert the time to a string
		std::stringstream ss;
		ss << now;
		return ss.str();
	}
}