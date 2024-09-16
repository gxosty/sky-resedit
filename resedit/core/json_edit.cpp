#include "json_edit.hpp"

#include <misc/Logger.h>

#include <nlohmann/json.hpp>

#include <vector>
#include <fstream>

namespace resedit::core
{
	enum class JsonPathElementType
	{
		OBJECT = 0,
		ARRAY
	};

	struct JsonPathElement
	{
		void* ptr = nullptr;
		JsonPathElementType type;
	};

	void _parse_json_path(
		std::vector<JsonPathElement>& output,
		nlohmann::json& json_data,
		const std::string& path,
		unsigned position = 0
	) {
		std::vector<JsonPathElement> temp;

		if (json_data.is_array())
		{
			output.emplace_back(json_data.get_ptr<nlohmann::json::array_t*>(), JsonPathElementType::ARRAY);
		}
		else
		{
			output.emplace_back(json_data.get_ptr<nlohmann::json::object_t*>(), JsonPathElementType::OBJECT);
		}

		while (path.size() > position)
		{
			if (path[position] == '[')
			{
				unsigned array_pos = -1;

				if (path[position + 1] != ']')
				{
					auto found = path.find(']', position);

					if (found == std::string::npos)
					{
						throw std::runtime_error("invalid path \"" + path + "\" at position " + std::to_string(position) + ". expected ']'");
					}

					array_pos = (unsigned)std::stoi(path.substr(position + 1, found));
					position = found + 2;
				}
				else
				{
					position += 3;
				}

				for (auto& object : output)
				{
					if (object.type == JsonPathElementType::OBJECT)
						throw std::runtime_error("invalid path \"" + path + "\". [] array match used on json object type");

					if (array_pos == -1)
					{
						for (size_t i = 0; i < (*(nlohmann::json::array_t*)object.ptr).size(); i++)
							// We assume that arrays store only objects, the downside of this parser
							temp.emplace_back(
								(*(nlohmann::json::array_t*)object.ptr)[i].get_ptr<nlohmann::json::object_t*>(),
								JsonPathElementType::OBJECT
							);
					}
					else
					{
						temp.emplace_back(
							(*(nlohmann::json::array_t*)object.ptr)[array_pos].get_ptr<nlohmann::json::object_t*>(),
							JsonPathElementType::OBJECT
						);
					}
				}

				output = temp;
				temp.clear();
			}
			else
			{
				auto found = path.find('/', position);
				std::string key = path.substr(position, found);

				for (auto& object : output)
				{
					if (object.type == JsonPathElementType::ARRAY)
						throw std::runtime_error("invalid path \"" + path + "\". \"" + key + "\" key match used on json array type");

					nlohmann::json::object_t& json_object = *(nlohmann::json::object_t*)object.ptr;
					if (json_object[key].is_array())
					{
						temp.emplace_back(json_object[key].get_ptr<nlohmann::json::array_t*>(), JsonPathElementType::ARRAY);
					}
					else
					{
						temp.emplace_back(json_object[key].get_ptr<nlohmann::json::object_t*>(), JsonPathElementType::OBJECT);
					}
				}

				output = temp;
				temp.clear();
				position = (found == std::string::npos ? found : found + 1);
			}
		}
	}

	JsonReplaceByKeyEdit::JsonReplaceByKeyEdit(
		const std::string& asset_path,
		const std::filesystem::path& file_path,
		int repeat,
		const std::string& compare_key,
		const std::string& path_to_object_in_asset,
		const std::string& path_to_object_in_file
	) : Edit(asset_path, file_path, repeat),
		_compare_key{compare_key},
		_path_to_object_in_asset{path_to_object_in_asset},
		_path_to_object_in_file{path_to_object_in_file} {}

	void JsonReplaceByKeyEdit::apply(const AssetData& asset_data)
	{
		_apply(asset_data);

		if (!--_repeat)
			_is_applied = true;
	}

	uint64_t JsonReplaceByKeyEdit::get_modified_size(const AssetData& asset_data)
	{
		_apply(asset_data);

		return *asset_data.written_len;
	}

	void JsonReplaceByKeyEdit::_apply(const AssetData& asset_data)
	{
		std::ifstream file(_file_path);

		if (!file.is_open())
		{
			return;
		}

		std::string asset_buffer(asset_data.buffer, asset_data.written_len[0]);
		nlohmann::json from_json = nlohmann::json::parse(asset_buffer);
		nlohmann::json to_json;
		file >> to_json;

		file.close();

		std::vector<JsonPathElement> from_output;
		std::vector<JsonPathElement> to_output;

		_parse_json_path(from_output, from_json, _path_to_object_in_asset);
		_parse_json_path(to_output, to_json, _path_to_object_in_file);

		for (auto& from_object_data : from_output)
		{
			nlohmann::json::object_t& from_object = *(nlohmann::json::object_t*)from_object_data.ptr;

			for (auto& to_object_data : to_output)
			{
				nlohmann::json::object_t& to_object = *(nlohmann::json::object_t*)to_object_data.ptr;

				if (from_object[_compare_key].is_string())
				{
					if (from_object[_compare_key].get<std::string>() != to_object[_compare_key].get<std::string>())
						continue;
				}
				else if (from_object[_compare_key].is_number())
				{
					if (from_object[_compare_key].get<float>() != to_object[_compare_key].get<float>())
						continue;
				}
				else
				{
					continue;
				}

				nlohmann::json from_object_json(from_object);
				nlohmann::json to_object_json(to_object);
				from_object_json.merge_patch(to_object_json);
				from_object = *from_object_json.get_ptr<nlohmann::json::object_t*>();
			}
		}

		std::string buffer = from_json.dump();
		memset(asset_data.buffer, 0, asset_data.max_len);
		strcpy(asset_data.buffer, buffer.c_str());
		asset_data.written_len[0] = buffer.size();
	}
}