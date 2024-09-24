#include <modui/ui/button.hpp>
#include <modui/app.hpp>

#include <cmath>
#include <cstdio>

namespace modui::ui
{
	Button::Button(const std::string& text) : BaseButton(),
		_font_size{ImGui::GetFontSize()},
		_press_factor{0.0f}
	{
		this->_size = Vec2(MODUI_SIZE_WIDTH_WRAP, utils::dp(40));
		this->_rounding = this->_size.y / 2.0f;
		this->set_text(text);
	}

	Button* Button::init(const std::string& text) { return new Button(text); }

	Button* Button::set_text(const std::string& text)
	{
		this->_text = text;

		if (modui::get_current_app()->is_rendering())
			this->_update_text_size();

		return this;
	}

	Button* Button::set_font_size(float font_size)
	{
		this->_font_size = font_size;

		if (modui::get_current_app()->is_rendering())
			this->_update_text_size();

		return this;
	}

	const std::string& Button::get_text()
	{
		return this->_text;
	}

	void Button::pre_render()
	{
		this->_update_text_size();

		Widget::pre_render();
	}

	void Button::render()
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		Theme& theme = this->get_theme();

		Vec2 size = this->_calculated_size;
		Vec2 pos = this->_pos;

		ui::BaseButton::render();

		this->_press_factor = utils::clamp(
			this->_press_factor + (ImGui::GetIO().DeltaTime * MODUI_WIDGET_PRESS_TRANSITION_SPEED) * (this->_is_held ? 1.0f : -1.0f),
			0.0f,
			1.0f
		);

		Col4 fill_color = utils::Col32to4(theme(ThemeColor::Primary));
		Col32 ripple_color = theme[ThemeColor::OnPrimary].get_alpha_applied(Theme::global_alpha * this->_press_factor * 0.1f);

		draw_list->AddRectFilled(
			pos,
			pos + size,
			utils::Col4to32(fill_color),
			this->_rounding
		);

		draw_list->AddRectFilled(
			pos,
			pos + size,
			ripple_color,
			this->_rounding
		);

		Vec2 text_pos = pos + (size - this->_text_size) / 2.0f;

		draw_list->AddText(ImGui::GetFont(), this->_font_size, text_pos, theme(ThemeColor::OnPrimary), this->_text.c_str());
	}

	void Button::_update_text_size()
	{
		if (this->_text.empty() || this->_font_size == 0.0f)
		{
			this->_text_size.x = 0.0f;
			this->_text_size.y = 0.0f;

			return;
		}

		this->_text_size = ImGui::GetFont()->CalcTextSizeA(this->_font_size, FLT_MAX, 0.0f, this->_text.c_str(), nullptr);
	}

	float Button::get_wrapped_size_x()
	{
		return this->_text_size.x + utils::dp(28) * 2.0f;
	}

	float Button::get_wrapped_size_y()
	{
		return this->_text_size.y + utils::dp(14) * 2.0f;
	}

	float Button::calculate_size_x(float bounding_box_size_x)
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

	float Button::calculate_size_y(float bounding_box_size_y)
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