#include "replace_edit.hpp"

#include <cstdlib>
#include <cstring>
#include <fstream>

namespace resedit::core
{
	ReplaceEdit::ReplaceEdit(
		const std::string& asset_path,
		const std::filesystem::path& file_path,
		int repeat
	) : Edit(asset_path, file_path, repeat) {}

	void ReplaceEdit::apply(const AssetData& asset_data)
	{
		std::ifstream file(_file_path);

		if (!file.is_open())
			return;

		memset(asset_data.buffer, 0, asset_data.max_len);
		file.read(asset_data.buffer, asset_data.max_len);
		asset_data.written_len[0] = file.gcount();

		file.close();

		if (!--_repeat)
			_is_applied = true;
	}
}