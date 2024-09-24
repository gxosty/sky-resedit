#pragma once

#include "../common.hpp"
#include "theme.hpp"

#include <string>
#include <unordered_map>

namespace modui
{
	class ThemeManager
	{
	public:
		ThemeManager() = default;

		ThemeID add(const Theme& theme);
		Theme&  create();
		Theme&  get(ThemeID id);
		void    remove(ThemeID Id);

	private:
		std::unordered_map<ThemeID, Theme> _themes;
	};
}