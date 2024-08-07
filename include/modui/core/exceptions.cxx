#include <modui/core/exceptions.hpp>

namespace modui::exceptions
{
	ThemeNotFoundException::ThemeNotFoundException(const std::string& theme_name)
		: std::runtime_error("Theme \"" + theme_name + "\" not found") {};

	AddWidgetException::AddWidgetException(const std::string& reason)
		: std::runtime_error(reason) {};

	ScreenNotFoundException::ScreenNotFoundException(const std::string& screen_name)
		: std::runtime_error("Screen \"" + screen_name + "\" not found") {};
}