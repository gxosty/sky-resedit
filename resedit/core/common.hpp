#pragma once

#include <string>
#include <cstdint>

#define RESEDIT_RESOURCE_PACK_METADATA_FILENAME "metadata.json"
#define RESEDIT_RESOURCE_PACK_DEFAULT_NAME "Unknown pack"
#define RESEDIT_RESOURCE_PACK_DEFAULT_AUTHOR "Unknown author"
#define RESEDIT_RESOURCE_PACK_DEFAULT_DESCRIPTION "[No description]"
#define RESEDIT_RESOURCE_PACK_DEFAULT_VERSION "version unspecified"
#define RESEDIT_RESOURCE_PACK_DEFAULT_SKY_VERSION 0

struct AssetData
{
	std::string const path;
	char* const buffer;
	uint64_t const max_len;
	uint64_t* const written_len;
};