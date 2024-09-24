#include <modui/core/exceptions.hpp>

namespace modui::exceptions
{
	ThemeNotFoundException::ThemeNotFoundException(uint32_t id)
		: std::runtime_error("Theme with id \"" + std::to_string(id) + "\" not found") {};

	AddThemeException::AddThemeException(const std::string& reason)
		: std::runtime_error(reason) {};

	AddWidgetException::AddWidgetException(const std::string& reason)
		: std::runtime_error(reason) {};

	ScreenNotFoundException::ScreenNotFoundException(const std::string& screen_name)
		: std::runtime_error("Screen \"" + screen_name + "\" not found") {};
}