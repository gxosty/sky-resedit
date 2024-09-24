#include "replace_edit.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

namespace resedit::core
{
	ReplaceEdit::ReplaceEdit(
		const std::string& asset_path,
		const std::filesystem::path& file_path,
		int repeat,
		bool is_injected
	) : Edit(asset_path, file_path, repeat, is_injected) {}

	void ReplaceEdit::apply(const AssetData& asset_data)
	{
		_apply(asset_data);

		if (!--_repeat)
			_is_applied = true;
	}

	uint64_t ReplaceEdit::get_modified_size(const AssetData& asset_data)
	{
		if (!_is_injected)
		{
			_apply(asset_data);
		}
		else
		{
			std::ifstream file(_file_path);
			if (!file.is_open()) return 0;

			file.seekg(0, std::ios::end);
			*asset_data.written_len = file.tellg();
			file.close();
		}

		return *asset_data.written_len;
	}

	void ReplaceEdit::_apply(const AssetData& asset_data)
	{
		std::ifstream file(_file_path);

		if (!file.is_open())
			return;

		memset(asset_data.buffer, 0, asset_data.max_len);
		file.read(asset_data.buffer, asset_data.max_len);
		asset_data.written_len[0] = file.gcount();

		file.close();
	}
}