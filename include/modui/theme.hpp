#pragma once

#include "core/common.hpp"

#include <string>
#include <unordered_map>

namespace modui
{
	// I like Material3 color converntion
	// even if I don't really understand them
	typedef struct __THEME_VARS__ {
		Col32 primary;
		Col32 on_primary;
		Col32 primary_container;
		Col32 on_primary_container;

		Col32 secondary;
		Col32 on_secondary;
		Col32 secondary_container;
		Col32 on_secondary_container;

		Col32 tertiary;
		Col32 on_tertiary;
		Col32 tertiary_container;
		Col32 on_tertiary_container;

		Col32 error;
		Col32 on_error;
		Col32 error_container;
		Col32 on_error_container;

		Col32 surface;
		Col32 on_surface;
		Col32 surface_variant;
		Col32 on_surface_variant;
		Col32 surface_container_highest;
		Col32 surface_container_high;
		Col32 surface_container;
		Col32 surface_container_low;
		Col32 surface_container_lowest;
		Col32 inverse_surface;
		Col32 inverse_on_surface;
		Col32 surface_tint;

		Col32 outline;
		Col32 outline_variant;
	} ThemeVars;

	class Theme
	{
	public:
		Theme(const std::string& theme_name);

		const std::string& get_name() const;
		ThemeVars& operator()();

	private:
		std::string _theme_name;
		ThemeVars _theme_vars;
	};

	class ThemeManager
	{
	public:
		ThemeManager() = default;

		void   add(const Theme& theme);
		Theme& get(const std::string& theme_name);
		void   remove(const std::string& theme_name);

	private:
		std::unordered_map<std::string, Theme> _themes;
	};
}