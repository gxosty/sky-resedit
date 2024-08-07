#include <modui/core/common.hpp>

namespace modui
{
	App* __current_app = nullptr;
	float __screen_density = 1.6667f;
	float __ui_scale = 1.0f;

	App* get_current_app()
	{
		return __current_app;
	}

	void set_screen_density(float density)
	{
		__screen_density = density;
	}

	void set_ui_scale(float scale)
	{
		__ui_scale = scale;
	}

	namespace utils
	{
		Col4 Col32to4(Col32 col32) { return ImGui::ColorConvertU32ToFloat4(col32); }
		Col32 Col4to32(Col4 col4) { return ImGui::ColorConvertFloat4ToU32(col4); }

		float dp(float size) { return size * __screen_density * __ui_scale; }
		float dp_raw(float size) { return size * __screen_density; }

		Col32 add_button_pressed_layer(Col32 base_color, Col32 press_layer)
		{
			Col4 press_layer_4 = Col32to4(press_layer);
			Col4 base_color_4 = Col32to4(base_color);

			return Col4to32(
				Col4{
					base_color_4.x + press_layer_4.x * 0.1f,
					base_color_4.y + press_layer_4.y * 0.1f,
					base_color_4.z + press_layer_4.z * 0.1f,
					base_color_4.w + press_layer_4.w * 0.1f
				}
			);
		}

		Col4 add_button_pressed_layer(Col4 base_color, Col4 press_layer)
		{
			return Col4{
				base_color.x + press_layer.x * 0.1f,
				base_color.y + press_layer.y * 0.1f,
				base_color.z + press_layer.z * 0.1f,
				base_color.w + press_layer.w * 0.1f
			};
		}
	}
}