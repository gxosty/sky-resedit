#include <modui/core/style/thememanager.hpp>

#include <random>

namespace modui
{
	ThemeID ThemeManager::add(const Theme& theme)
	{
		if (this->_themes.contains(theme.get_id()))
		{
			throw exceptions::AddThemeException("Theme with the same ThemeID already exists");
		}

		this->_themes.insert({theme.get_id(), theme});

		return theme.get_id();
	}

	Theme& ThemeManager::create()
	{
		ThemeID theme_id;

		do {
			theme_id = rand() % UINT32_MAX;
		} while (this->_themes.contains(theme_id));

		this->_themes.insert({theme_id, Theme(theme_id)});
		return this->_themes.at(theme_id);
	}

	Theme& ThemeManager::get(ThemeID id)
	{
		if (this->_themes.contains(id))
		{
			return this->_themes.at(id);
		}

		throw exceptions::ThemeNotFoundException(id);
	}

	void ThemeManager::remove(ThemeID id)
	{
		this->_themes.erase(id);
	}
}