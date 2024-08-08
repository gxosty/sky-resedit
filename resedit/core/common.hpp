#pragma once

#include <string>

struct AssetData
{
	std::string const path;
	char* const buffer;
	unsigned const max_len;
	unsigned* const written_len;
};