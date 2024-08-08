#pragma once

#include <string>

#define RESEDIT_RESOURCE_PACK_METADATA_FILENAME "metadata.json"
#define RESEDIT_RESOURCE_PACK_DEFAULT_NAME "Unknown pack"
#define RESEDIT_RESOURCE_PACK_DEFAULT_AUTHOR "Unknown author"
#define RESEDIT_RESOURCE_PACK_DEFAULT_SKY_VERSION 0

struct AssetData
{
	std::string const path;
	char* const buffer;
	unsigned const max_len;
	unsigned* const written_len;
};