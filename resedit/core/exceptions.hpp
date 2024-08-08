#pragma once

#include <string>
#include <filesystem>
#include <stdexcept>

namespace resedit::core::exceptions
{
	class ResourcePackMissingMetadata : public std::runtime_error
	{
	public:
		ResourcePackMissingMetadata(std::filesystem::path resource_pack_path)
			: std::runtime_error("Missing metadata in -> " + resource_pack_path.string()) {}
	};

	class ResourcePackInvalidEdit : public std::runtime_error
	{
	public:
		ResourcePackInvalidEdit(std::string resource_pack_name, int edit_index = -1) : std::runtime_error("")
		{
			_error_smg = "Invalid edit in pack -> " + resource_pack_name;

			if (edit_index != -1)
				_error_smg += " (index: " + std::to_string(edit_index) + ")";
		}

		const char* what() const noexcept override
		{
			return _error_smg.c_str();
		}

	private:
		std::string _error_smg;
	};
}