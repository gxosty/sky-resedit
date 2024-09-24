#include <modui/core/style/theme.hpp>

namespace modui
{
	float Theme::global_alpha = 1.0;

	Theme::Theme(ThemeID _id)
		: _id{_id},
		  _colors{
			Color(MODUI_COLOR_RGBA(0x67, 0x50, 0xA4, 0xFF)), // Primary
			Color(MODUI_COLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF)), // OnPrimary
			Color(MODUI_COLOR_RGBA(0xEA, 0xDD, 0xFF, 0xFF)), // PrimaryContainer
			Color(MODUI_COLOR_RGBA(0x21, 0x00, 0x5D, 0xFF)), // OnPrimaryContainer

			Color(MODUI_COLOR_RGBA(0x62, 0x5B, 0x71, 0xFF)), // Secondary
			Color(MODUI_COLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF)), // OnSecondary
			Color(MODUI_COLOR_RGBA(0xE8, 0xDE, 0xF8, 0xFF)), // SecondaryContainer
			Color(MODUI_COLOR_RGBA(0x1D, 0x19, 0x2B, 0xFF)), // OnSecondaryContainer

			Color(MODUI_COLOR_RGBA(0x7D, 0x52, 0x60, 0xFF)), // Tertiary
			Color(MODUI_COLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF)), // OnTertiary
			Color(MODUI_COLOR_RGBA(0xFF, 0xD8, 0xE4, 0xFF)), // TertiaryContainer
			Color(MODUI_COLOR_RGBA(0x31, 0x11, 0x1D, 0xFF)), // OnTertiaryContainer

			Color(MODUI_COLOR_RGBA(0xB3, 0x26, 0x1E, 0xFF)), // Error
			Color(MODUI_COLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF)), // OnError
			Color(MODUI_COLOR_RGBA(0xF9, 0xDE, 0xDC, 0xFF)), // ErrorContainer
			Color(MODUI_COLOR_RGBA(0x41, 0x0E, 0x0B, 0xFF)), // OnErrorContainer

			Color(MODUI_COLOR_RGBA(0xFE, 0xF7, 0xFF, 0xFF)), // Surface
			Color(MODUI_COLOR_RGBA(0x1D, 0x1B, 0x20, 0xFF)), // OnSurface
			Color(MODUI_COLOR_RGBA(0xE7, 0xE0, 0xEC, 0xFF)), // SurfaceVariant
			Color(MODUI_COLOR_RGBA(0x49, 0x45, 0x4F, 0xFF)), // OnSurfaceVariant
			Color(MODUI_COLOR_RGBA(0xE6, 0xE0, 0xE9, 0xFF)), // SurfaceContainerHighest
			Color(MODUI_COLOR_RGBA(0xEC, 0xE6, 0xF0, 0xFF)), // SurfaceContainerHigh
			Color(MODUI_COLOR_RGBA(0xF3, 0xED, 0xF7, 0xFF)), // SurfaceContainer
			Color(MODUI_COLOR_RGBA(0xF7, 0xF2, 0xFA, 0xFF)), // SurfaceContainerLow
			Color(MODUI_COLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF)), // SurfaceContainerLowest
			Color(MODUI_COLOR_RGBA(0x32, 0x2F, 0x35, 0xFF)), // InverseSurface
			Color(MODUI_COLOR_RGBA(0xF5, 0xEF, 0xF7, 0xFF)), // InverseOnSurface
			Color(MODUI_COLOR_RGBA(0x67, 0x50, 0xA4, 0xFF)), // SurfaceTint

			Color(MODUI_COLOR_RGBA(0x79, 0x74, 0x7E, 0xFF)), // Outline
			Color(MODUI_COLOR_RGBA(0xCA, 0xC4, 0xD0, 0xFF))  // OutlineVariant
		  } {}

	ThemeID Theme::get_id() const
	{
		return this->_id;
	}

	Col32 Theme::operator()(ThemeColor color)
	{
		return this->_colors[static_cast<unsigned>(color)].get_alpha_applied(Theme::global_alpha);
	}

	Color& Theme::operator[](ThemeColor color)
	{
		return this->_colors[static_cast<unsigned>(color)];
	}
}