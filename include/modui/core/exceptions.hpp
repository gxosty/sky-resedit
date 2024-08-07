#pragma once

#include <stdexcept>
#include <string>

namespace modui::exceptions
{
	class ThemeNotFoundException : public std::runtime_error
	{
	public:
		ThemeNotFoundException(const std::string& theme_name);
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