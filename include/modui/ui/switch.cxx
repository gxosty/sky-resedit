#include <modui/ui/switch.hpp>
#include "../internal/internal_core.hpp"

#include <cstdio>

namespace modui::ui
{
	Switch::Switch(bool state) : BaseButton(),
		_state{state},
		_press_factor{0.0f},
		_state_factor{0.0f}
	{
		this->_size = Vec2(utils::dp(52), utils::dp(40));
		BaseButton::on_release(MODUI_CALLBACK(this) {
			this->set_state(!this->get_state());
		});
	}

	Switch* Switch::init(bool state) { return new Switch(state); }

	Switch* Switch::set_state(bool state)
	{
		this->_state = state;

		return this;
	}

	bool Switch::get_state()
	{
		return this->_state;
	}

	Widget* Switch::on_release(ButtonInputCallback callback)
	{
		BaseButton::on_release(MODUI_CALLBACK(this, callback) {
			this->set_state(!this->get_state());
			callback(this);
		});

		return this;
	}

	void Switch::render()
	{
		const Vec2 __track_size = Vec2(utils::dp(52), utils::dp(32));
		const float __outline_width = utils::dp(2);
		const float __ripple_radius = utils::dp(20);

		ImDrawList* draw_list =  ImGui::GetWindowDrawList();
		Theme& theme = this->get_theme();

		ui::BaseButton::render();

		this->_state_factor = utils::clamp(
			this->_state_factor + (ImGui::GetIO().DeltaTime * MODUI_WIDGET_PRESS_TRANSITION_SPEED) * (this->_state ? 1.0f : -1.0f),
			0.0f,
			1.0f
		);

		this->_press_factor = utils::clamp(
			this->_press_factor + (ImGui::GetIO().DeltaTime * MODUI_WIDGET_PRESS_TRANSITION_SPEED) * (this->_is_held ? 1.0f : -1.0f),
			0.0f,
			1.0f
		);

		Col32 ripple_color;
		Col4 outline_color, handle_color;

		ripple_color = (theme().primary & 0xFFFFFF) | (unsigned(0xFF * this->_press_factor * 0.1f) << 24);

		Col4 fill_color = utils::mix(
			utils::Col32to4(theme().surface_container_highest),
			utils::Col32to4(theme().primary),
			this->_state_factor
		);

		float handle_radius = utils::mix(
			(this->_state ? utils::dp(12) : utils::dp(8)),
			utils::dp(14),
			this->_press_factor
		);

		outline_color = utils::mix(
			utils::Col32to4(theme().outline),
			Col4(0, 0, 0, 0),
			this->_state_factor
		);

		handle_color = utils::mix(
			utils::Col32to4(theme().outline),
			utils::Col32to4(theme().on_primary),
			this->_state_factor
		);

		Vec2 track_pos = this->_pos + (this->_calculated_size - __track_size) / 2.0f;
		Vec2 handle_pos = track_pos + Vec2(__track_size.x - __track_size.y / 2.0f - (__track_size.x - __track_size.y) * (1.0f - this->_state_factor), __track_size.y / 2.0f);

		draw_list->AddRectFilled(track_pos, track_pos + __track_size, utils::Col4to32(fill_color), MODUI_ROUNDING_FULL);
		draw_list->AddRect(track_pos, track_pos + __track_size, utils::Col4to32(outline_color), MODUI_ROUNDING_FULL, 0, __outline_width);
		draw_list->AddCircleFilled(handle_pos, handle_radius, utils::Col4to32(handle_color));

		draw_list->AddCircleFilled(handle_pos, __ripple_radius, ripple_color);
	}

	float Switch::get_wrapped_size_x()
	{
		return utils::dp(52) + this->_padding.y + this->_padding.w;
	}

	float Switch::get_wrapped_size_y()
	{
		return utils::dp(32) + this->_padding.x + this->_padding.z;
	}

	float Switch::calculate_size_x(float bounding_box_size_x)
	{
		float x = this->_size.x;
		this->_bounding_box_size.x = bounding_box_size_x;

		if (x == MODUI_SIZE_WIDTH_FULL)
		{
			x = bounding_box_size_x;
		}
		else if (x == MODUI_SIZE_WIDTH_WRAP)
		{
			x = this->get_wrapped_size_x();
		}
		else if (x < 0.0f)
		{
			x = bounding_box_size_x + x;
		}

		this->_calculated_size.x = x;

		return x;
	}

	float Switch::calculate_size_y(float bounding_box_size_y)
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