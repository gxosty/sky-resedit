#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "exceptions.hpp"

#include <functional>
#include <cstdint>

#define __MODUI_VERSION__ "0.1.0beta"

typedef ImVec2 Vec2;
typedef ImVec4 Vec4;

typedef ImU32 Col32;
typedef ImVec4 Col4;

namespace modui::image { class Image; }
typedef modui::image::Image* ImageID;
typedef uint32_t ThemeID;

static constexpr char* DEFAULT_ID = (char*)"modui_id";
static constexpr ThemeID DEFAULT_THEME_LIGHT = 0x77777777;
static constexpr ThemeID DEFAULT_THEME_DARK = 0x77777778;

// tells that virtual functions exists just to not break the framework
#define MODUI_VIRTUAL_PLACEHOLDER virtual

#define MODUI_WIDGET_INIT(class_name, ...) (class_name*)((new class_name(__VA_ARGS__))->build_widget())

#define MODUI_CALLBACK(...) [__VA_ARGS__](modui::ui::Widget* this_widget)
#define MODUI_EMPTY_CALLBACK [](Widget*){}
#define MODUI_ANIMATION_FINISH_CALLBACK(...) [__VA_ARGS__]()

#define MODUI_SIZE_WRAP 0.0f
#define MODUI_SIZE_WIDTH_WRAP MODUI_SIZE_WRAP
#define MODUI_SIZE_HEIGHT_WRAP MODUI_SIZE_WRAP

#define MODUI_SIZE_FULL -1.0f
#define MODUI_SIZE_WIDTH_FULL MODUI_SIZE_FULL
#define MODUI_SIZE_HEIGHT_FULL MODUI_SIZE_FULL

#define MODUI_ROUNDING_FULL 99999.0f
#define MODUI_WIDGET_PRESS_TRANSITION_SPEED 1 / 0.08f // 80 millis

#define DP(x) ::modui::utils::dp(x)

// Hex is ARGB
#define MODUI_COLOR_HEX(hx) ((hx & 0xFF000000) | ((hx >> 16) & 0xFF) | (hx & 0xFF00) | ((hx & 0xFF) << 16))
#define MODUI_COLOR_RGBA(r, g, b, a) (Col32)((a << 24) | (b << 16) | (g << 8) | r)

namespace modui { class App; }
namespace modui::ui { class Widget; }
typedef std::function<void(modui::ui::Widget* this_widget)> ButtonInputCallback;
typedef std::function<void(void)> AnimationFinishCallback;

namespace modui
{
	// I like Material3 color convention
	// even if I don't really understand them
	enum class ThemeColor
	{
		Primary = 0,
		OnPrimary,
		PrimaryContainer,
		OnPrimaryContainer,

		Secondary,
		OnSecondary,
		SecondaryContainer,
		OnSecondaryContainer,

		Tertiary,
		OnTertiary,
		TertiaryContainer,
		OnTertiaryContainer,

		Error,
		OnError,
		ErrorContainer,
		OnErrorContainer,

		Surface,
		OnSurface,
		SurfaceVariant,
		OnSurfaceVariant,
		SurfaceContainerHighest,
		SurfaceContainerHigh,
		SurfaceContainer,
		SurfaceContainerLow,
		SurfaceContainerLowest,
		InverseSurface,
		InverseOnSurface,
		SurfaceTint,

		Outline,
		OutlineVariant,

		#ifdef MODUI_CUSTOM_THEME_COLORS
			MODUI_CUSTOM_THEME_COLORS
		#endif

		ThemeColorSize
	};

	enum Side {
		SIDE_TOP,
		SIDE_TOP_RIGHT,
		SIDE_RIGHT,
		SIDE_BOTTOM_RIGHT,
		SIDE_BOTTOM,
		SIDE_BOTTOM_LEFT,
		SIDE_LEFT,
		SIDE_TOP_LEFT
	};

	App* get_current_app();
	void set_screen_density(float density);
	void set_ui_scale(float scale);

	namespace utils
	{
		Col4 Col32to4(Col32 col32);
		Col32 Col4to32(Col4 col4);

		float dp(float size);
		float dp_raw(float size);

		Col32 add_button_pressed_layer(Col32 base_color, Col32 press_layer);
		Col4 add_button_pressed_layer(Col4 base_color, Col4 press_layer);

		static inline Col4 mix(Col4 col1, Col4 col2, float factor) { return ImLerp(col1, col2, factor); }
		static inline unsigned mix(unsigned v1, unsigned v2, float factor) { return (v1 + (v2 - v1) * factor); }

		static inline float clamp(float value, float minv, float maxv) { return (value < minv) ? minv : (value > maxv) ? maxv : value; }

		static inline float map(float value, float a_min, float a_max, float b_min, float b_max) { return (value - a_min) * (b_max - b_min) / (a_max - a_min) + b_min; }
		static inline uint64_t map(uint64_t value, uint64_t a_min, uint64_t a_max, uint64_t b_min, uint64_t b_max) { return (value - a_min) * (b_max - b_min) / (a_max - a_min) + b_min; }
	}
}

#ifdef MODUI_SHOW_BOUNDING_BOXES
	#define _MODUI_SHOW_BB(w) w->render_bounding_box();
#else
	#define _MODUI_SHOW_BB(x) 0;
#endif // MODUI_SHOW_BOUNDING_BOX