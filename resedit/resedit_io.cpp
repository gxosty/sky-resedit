#include "resedit_io.hpp"

#include <Cipher/Cipher.h>

#define RESEDIT_BASE_DIR_NAME "resedit"
#define RESEDIT_CONFIG_FILE_NAME "config.json"
#define RESEDIT_RESOURCE_PACKS_PATH "packs"
#define RESEDIT_TEMP_DIR_NAME "temp"

namespace resedit::io
{
	namespace fs = std::filesystem;

	inline fs::path _get_base_directory()
	{
		return fs::path(Cipher::getConfigPath()) / RESEDIT_BASE_DIR_NAME;
	}

	fs::path get_config_path()
	{
		return _get_base_directory() / RESEDIT_CONFIG_FILE_NAME;
	}

	fs::path get_resource_pack_path(const std::string& pack_id)
	{
		return _get_base_directory() / RESEDIT_RESOURCE_PACKS_PATH / pack_id;
	}

	fs::path get_temp_directory_path()
	{
		return _get_base_directory() / RESEDIT_TEMP_DIR_NAME;
	}
}