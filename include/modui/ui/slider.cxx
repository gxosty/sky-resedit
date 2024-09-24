#include <modui/ui/slider.hpp>
#include "../internal/internal_core.hpp"

#include <cstdio>

namespace modui::ui
{
	Slider::Slider(float min_value, float max_value) : BaseSlider(min_value, max_value, utils::dp(20)),
		_press_factor{0.0f}
	{
		this->_size = Vec2(MODUI_SIZE_WIDTH_FULL, utils::dp(40));
	}

	Slider* Slider::init(float min_value, float max_value) { return new Slider(min_value, max_value); }

	void Slider::render()
	{
		const float __ripple_radius = utils::dp(20);
		const float __circle_radius = utils::dp(10);
		const float __slider_thickness = utils::dp(6);

		ImDrawList* draw_list =  ImGui::GetWindowDrawList();
		Theme& theme = this->get_theme();

		Vec2 pos = this->_pos;

		pos.x += this->_padding.w;
		pos.y += this->_padding.x;

		Vec2 size = this->_calculated_size;

		float start_x = pos.x + __ripple_radius;
		float end_x = pos.x + size.x - __ripple_radius;
		float center_y = pos.y + size.y / 2.0f;

		float slider_x = utils::map(this->_value, this->_min_value, this->_max_value, start_x, end_x);\

		Vec2 l1_p1 = Vec2(start_x, center_y - __slider_thickness / 2.0f);
		Vec2 l1_p2 = Vec2(slider_x, center_y + __slider_thickness / 2.0f);

		Vec2 l2_p1 = Vec2(slider_x, center_y - __slider_thickness / 2.0f);
		Vec2 l2_p2 = Vec2(end_x, center_y + __slider_thickness / 2.0f);

		ui::BaseSlider::render();

		this->_press_factor = utils::clamp(
			this->_press_factor + (ImGui::GetIO().DeltaTime * MODUI_WIDGET_PRESS_TRANSITION_SPEED) * (this->_is_held ? 1.0f : -1.0f),
			0.0f,
			1.0f
		);

		Col4 fill_color = utils::mix(
			utils::Col32to4(theme(ThemeColor::Primary)),
			utils::add_button_pressed_layer(
				utils::Col32to4(theme(ThemeColor::Primary)),
				utils::Col32to4(theme(ThemeColor::OnPrimary))
			),
			this->_press_factor
		);

		Col32 ripple_color = theme[ThemeColor::Primary].get_alpha_applied(Theme::global_alpha * this->_press_factor * 0.1f);

		draw_list->AddRectFilled(Vec2(slider_x - __ripple_radius, center_y - __ripple_radius), Vec2(slider_x + __ripple_radius, center_y + __ripple_radius), ripple_color, 99999.0f);
		draw_list->AddRectFilled(l1_p1, l1_p2, theme(ThemeColor::Primary), MODUI_ROUNDING_FULL);
		draw_list->AddRectFilled(l2_p1, l2_p2, theme[ThemeColor::Primary].get_alpha_applied(Theme::global_alpha * 0.38f), MODUI_ROUNDING_FULL);
		draw_list->AddCircleFilled(Vec2(slider_x, center_y), __circle_radius, theme(ThemeColor::Primary));
	}

	float Slider::get_wrapped_size_x()
	{
		return utils::dp(88);
	}

	float Slider::get_wrapped_size_y()
	{
		return utils::dp(40) + this->_padding.x + this->_padding.z;
	}

	float Slider::calculate_size_x(float bounding_box_size_x)
	{
		float x = this->_size.x;
		this->_bounding_box_size.x = bounding_box_size_x;

		if ((x == MODUI_SIZE_WIDTH_FULL) || (x == MODUI_SIZE_WIDTH_WRAP))
		{
			x = bounding_box_size_x;
		}
		else if (x < 0.0f)
		{
			x = bounding_box_size_x + x;
		}

		this->_calculated_size.x = x;

		return x;
	}

	float Slider::calculate_size_y(float bounding_box_size_y)
	{
		float y = this->_size.y;
		this->_bounding_box_size.y = bounding_box_size_y;

		if (y == MODUI_SIZE_WIDTH_FULL)
		{
			y = bounding_box_size_y;
		}
		else if (y == MODUI_SIZE_HEIGHT_WRAP)
		{
			y = this->get_wrapped_size_y(); 
		}
		else if (y < 0.0f)
		{
			y = bounding_box_size_y + y;
		}

		this->_calculated_size.y = y;

		return y;
	}
}