#pragma once

#include <stdexcept>
#include <string>
#include <cstdint>

namespace modui::exceptions
{
	class ThemeNotFoundException : public std::runtime_error
	{
	public:
		ThemeNotFoundException(uint32_t id);
	};

	class AddThemeException : public std::runtime_error
	{
	public:
		AddThemeException(const std::string& reason);
	};

	class AddWidgetException : public std::runtime_error
	{
	public:
		AddWidgetException(const std::string& reason);
	};

	class ScreenNotFoundException : public std::runtime_error
	{
	public:
		ScreenNotFoundException(const std::string& screen_name);
	};
}