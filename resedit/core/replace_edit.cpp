#include "replace_edit.hpp"

namespace resedit::core
{
	ReplaceEdit::ReplaceEdit(
		const std::string& asset_path,
		const std::filesystem::path& file_path
	) : Edit(asset_path, file_path) {}

	void ReplaceEdit::apply(const AssetData& asset_data)
	{
		return;
	}
}