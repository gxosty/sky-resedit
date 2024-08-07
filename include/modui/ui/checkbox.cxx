#include <modui/ui/checkbox.hpp>
#include "../internal/internal_core.hpp"

namespace modui::ui
{
	Checkbox::Checkbox(bool state) : BaseButton(),
		_state{state},
		_rounding{utils::dp(2)}
	{
		this->_size = Vec2(utils::dp(40), utils::dp(40));
		BaseButton::on_release(MODUI_CALLBACK(this) {
			this->set_state(!this->get_state());
		});
	}

	Checkbox* Checkbox::init(bool state) { return new Checkbox(state); }

	Checkbox* Checkbox::set_state(bool state)
	{
		this->_state = state;

		return this;
	}

	bool Checkbox::get_state()
	{
		return this->_state;
	}

	Checkbox* Checkbox::on_release(ButtonInputCallback callback)
	{
		BaseButton::on_release(MODUI_CALLBACK(this, callback) {
			this->set_state(!this->get_state());
			callback(this);
		});

		return this;
	}

	void Checkbox::render()
	{
		const Vec2 __box_size = Vec2(utils::dp(18), utils::dp(18));
		const float __outline_width = utils::dp(2);

		ImDrawList* draw_list =  ImGui::GetWindowDrawList();
		Theme& theme = this->get_theme();

		Vec2 size = this->_calculated_size;
		Vec2 pos = this->_pos;

		ui::BaseButton::render();

		this->_press_factor = utils::clamp(
			this->_press_factor + (ImGui::GetIO().DeltaTime * MODUI_WIDGET_PRESS_TRANSITION_SPEED) * (this->_is_held ? 1.0f : -1.0f),
			0.0f,
			1.0f
		);

		Col4 outline_color, fill_color;
		Col32 ripple_color;

		ripple_color = (theme().primary & 0xFFFFFF) | (unsigned(0xFF * this->_press_factor * 0.1f) << 24);
		outline_color = utils::Col32to4(ripple_color);

		if (this->_state)
		{
			fill_color = utils::Col32to4(theme().primary);
			outline_color = utils::Col32to4(theme().primary);
		}
		else
		{
			fill_color = Vec4(0.0f, 0.0f, 0.0f, 0.0f);

			outline_color = utils::mix(
				utils::Col32to4(theme().outline),
				utils::add_button_pressed_layer(
					utils::Col32to4(theme().outline),
					utils::Col32to4(theme().outline_variant)
				),
				this->_press_factor
			);
		}

		Vec2 box_pos = pos + (size - __box_size) / 2.0f;

		draw_list->AddRectFilled(pos, pos + size, ripple_color, 99999.0f);

		draw_list->AddRectFilled(box_pos, box_pos + __box_size, utils::Col4to32(fill_color), this->_rounding);
		draw_list->AddRect(box_pos, box_pos + __box_size, utils::Col4to32(outline_color), this->_rounding, 0, __outline_width);

		if (this->_state)
		{
			Vec2 l1_p1 = Vec2(box_pos.x + utils::dp(4), box_pos.y + __box_size.y / 2.0f);
			Vec2 l1_p2 = Vec2(box_pos.x + __box_size.x / 2.0f - utils::dp(2), box_pos.y + __box_size.y - utils::dp(6));

			Vec2 l2_p1 = l1_p2;
			Vec2 l2_p2 = Vec2(box_pos.x + __box_size.x - utils::dp(4), box_pos.y + utils::dp(4));

			draw_list->AddLine(l1_p1, l1_p2, theme().on_primary, __outline_width);
			draw_list->AddLine(l2_p1, l2_p2, theme().on_primary, __outline_width);
		}
	}

	float Checkbox::get_wrapped_size_x()
	{
		return utils::dp(40) + this->_padding.y + this->_padding.w;
	}

	float Checkbox::get_wrapped_size_y()
	{
		return utils::dp(40) + this->_padding.x + this->_padding.z;
	}

	float Checkbox::calculate_size_x(float bounding_box_size_x)
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

	float Checkbox::calculate_size_y(float bounding_box_size_y)
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