#include "append_edit.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

namespace resedit::core
{
	AppendEdit::AppendEdit(
		const std::string& asset_path,
		const std::filesystem::path& file_path,
		int repeat,
		bool is_injected
	) : Edit(asset_path, file_path, repeat, is_injected) {}

	void AppendEdit::apply(const AssetData& asset_data)
	{
		_apply(asset_data);

		if (!--_repeat)
			_is_applied = true;
	}

	uint64_t AppendEdit::get_modified_size(const AssetData& asset_data)
	{
		_apply(asset_data);
		return *asset_data.written_len;
	}

	void AppendEdit::_apply(const AssetData& asset_data)
	{
		std::ifstream file(_file_path);

		if (!file.is_open())
			return;

		file.read(
			asset_data.buffer + asset_data.written_len[0],
			asset_data.max_len - asset_data.written_len[0]
		);

		asset_data.written_len[0] += file.gcount();
		asset_data.buffer[asset_data.written_len[0]] = 0;

		file.close();
	}
}