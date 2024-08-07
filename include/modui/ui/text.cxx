#include <modui/ui/text.hpp>
#include "../internal/internal_core.hpp"

#include <stdio.h>

namespace modui::ui
{
	Text::Text(const std::string& text) : Widget(),
		_text{text},
		_font_size{ImGui::GetFontSize()}
	{
		this->_size = Vec2(0.0f, 0.0f);
		this->_side = modui::SIDE_LEFT;
	}

	Text* Text::init(const std::string& text) { return new Text(text); }

	Text* Text::set_side(modui::Side side)
	{
		this->_side = side;

		return this;
	}

	Text* Text::set_text(const std::string& text)
	{
		this->_text = text;

		if (modui::get_current_app()->is_rendering())
			this->_update_text_size();

		return this;
	}

	Text* Text::set_font_size(float font_size)
	{
		this->_font_size = font_size;

		if (modui::get_current_app()->is_rendering())
			this->_update_text_size();

		return this;
	}

	void Text::pre_render()
	{
		this->_update_text_size();

		Widget::pre_render();
	}

	void Text::render()
	{
		ImDrawList* draw_list =  ImGui::GetWindowDrawList();
		Theme& theme = this->get_theme();

		Vec2 text_pos = this->_pos + (this->_calculated_size - this->_text_size) / 2.0f;
		text_pos.x += this->_padding.w;
		text_pos.y += this->_padding.x;

		draw_list->AddText(ImGui::GetFont(), this->_font_size, text_pos, this->is_on_card() ? theme().on_surface_variant : theme().on_surface, this->_text.c_str(), nullptr, this->_calculated_size.x);
	}

	void Text::_update_text_size(float available_width)
	{
		if (this->_text.empty() || (this->_font_size == 0.0f) || (available_width < 0.0f))
		{
			this->_text_size.x = 0.0f;
			this->_text_size.y = this->_font_size;

			return;
		}

		this->_text_size = ImGui::GetFont()->CalcTextSizeA(this->_font_size, FLT_MAX, available_width, this->_text.c_str(), nullptr);
		this->_text_size.x += 1.0f;
	}

	Vec2 Text::_calc_side()
	{
		return Vec2(0.0f, 0.0f);
	}

	float Text::get_wrapped_size_x()
	{
		return this->_text_size.x + this->_padding.y + this->_padding.w;
	}

	float Text::get_wrapped_size_y()
	{
		return this->_text_size.y + this->_padding.x + this->_padding.z;
	}

	float Text::calculate_size_x(float bounding_box_size_x)
	{
		float x = this->_size.x;
		this->_bounding_box_size.x = bounding_box_size_x;

		if (x == MODUI_SIZE_WIDTH_FULL)
		{
			x = bounding_box_size_x;
			this->_update_text_size(x - this->_padding.y - this->_padding.w);
		}
		else if (x == MODUI_SIZE_WIDTH_WRAP)
		{
			this->_update_text_size(bounding_box_size_x - this->_padding.y - this->_padding.w);
			x = this->get_wrapped_size_x();
		}
		else if (x < 0.0f)
		{
			x = bounding_box_size_x + x;
			this->_update_text_size(x - this->_padding.y - this->_padding.w);
		}

		this->_calculated_size.x = x;

		return x;
	}

	float Text::calculate_size_y(float bounding_box_size_y)
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

	bool Text::is_on_card()
	{
		return Widget::is_on_card();
	}

	TitleText::TitleText(const std::string& text) : Text(text)
	{
		this->set_font_size(ImGui::GetFontSize() * 1.15f);
	}

	TitleText* TitleText::init(const std::string& text) { return new TitleText(text); }

	bool TitleText::is_on_card() { return false; }
}