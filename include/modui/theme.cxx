#include <modui/theme.hpp>

namespace modui
{
	Theme::Theme(const std::string& theme_name) : 
		_theme_name{theme_name},
		_theme_vars{
			MODUI_COLOR_RGBA(0x67, 0x50, 0xA4, 0xFF), // primary;
			MODUI_COLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), // on_primary;
			MODUI_COLOR_RGBA(0xEA, 0xDD, 0xFF, 0xFF), // primary_container;
			MODUI_COLOR_RGBA(0x21, 0x00, 0x5D, 0xFF), // on_primary_container;

			MODUI_COLOR_RGBA(0x62, 0x5B, 0x71, 0xFF), // secondary;
			MODUI_COLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), // on_secondary;
			MODUI_COLOR_RGBA(0xE8, 0xDE, 0xF8, 0xFF), // secondary_container;
			MODUI_COLOR_RGBA(0x1D, 0x19, 0x2B, 0xFF), // on_secondary_container;

			MODUI_COLOR_RGBA(0x7D, 0x52, 0x60, 0xFF), // tertiary;
			MODUI_COLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), // on_tertiary;
			MODUI_COLOR_RGBA(0xFF, 0xD8, 0xE4, 0xFF), // tertiary_container;
			MODUI_COLOR_RGBA(0x31, 0x11, 0x1D, 0xFF), // on_tertiary_container;

			MODUI_COLOR_RGBA(0xB3, 0x26, 0x1E, 0xFF), // error;
			MODUI_COLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), // on_error;
			MODUI_COLOR_RGBA(0xF9, 0xDE, 0xDC, 0xFF), // error_container;
			MODUI_COLOR_RGBA(0x41, 0x0E, 0x0B, 0xFF), // on_error_container;

			MODUI_COLOR_RGBA(0xFE, 0xF7, 0xFF, 0xFF), // surface;
			MODUI_COLOR_RGBA(0x1D, 0x1B, 0x20, 0xFF), // on_surface;
			MODUI_COLOR_RGBA(0xE7, 0xE0, 0xEC, 0xFF), // surface_variant;
			MODUI_COLOR_RGBA(0x49, 0x45, 0x4F, 0xFF), // on_surface_variant;
			MODUI_COLOR_RGBA(0xE6, 0xE0, 0xE9, 0xFF), // surface_container_highest;
			MODUI_COLOR_RGBA(0xEC, 0xE6, 0xF0, 0xFF), // surface_container_high;
			MODUI_COLOR_RGBA(0xF3, 0xED, 0xF7, 0xFF), // surface_container;
			MODUI_COLOR_RGBA(0xF7, 0xF2, 0xFA, 0xFF), // surface_container_low;
			MODUI_COLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF), // surface_container_lowest;
			MODUI_COLOR_RGBA(0x32, 0x2F, 0x35, 0xFF), // inverse_surface;
			MODUI_COLOR_RGBA(0xF5, 0xEF, 0xF7, 0xFF), // inverse_on_surface;
			MODUI_COLOR_RGBA(0x67, 0x50, 0xA4, 0xFF), // surface_tint;

			MODUI_COLOR_RGBA(0x79, 0x74, 0x7E, 0xFF), // outline;
			MODUI_COLOR_RGBA(0xCA, 0xC4, 0xD0, 0xFF)  // outline_variant;
		} {};

	const std::string& Theme::get_name() const
	{
		return this->_theme_name;
	}

	ThemeVars& Theme::operator()()
	{
		return this->_theme_vars;
	}

	void ThemeManager::add(const Theme& theme)
	{
		if (this->_themes.contains(theme.get_name()))
		{
			this->remove(theme.get_name());
		}

		this->_themes.insert({theme.get_name(), theme});
	}

	Theme& ThemeManager::get(const std::string& theme_name)
	{
		if (!this->_themes.contains(theme_name))
		{
			throw new exceptions::ThemeNotFoundException(theme_name);
		}

		return this->_themes.at(theme_name);
	}

	void ThemeManager::remove(const std::string& theme_name)
	{
		if (this->_themes.contains(theme_name))
		{
			this->_themes.erase(theme_name);
		}
	}
}