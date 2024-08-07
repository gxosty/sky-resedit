#include <modui/ui/memorytext.hpp>
#include <modui/app.hpp>

#include <cstring>

namespace modui::ui
{
	MemoryText::MemoryText(const char* text) : Widget(),
		_text{text},
		_font_size{ImGui::GetFontSize()}
	{
		this->_size = Vec2(MODUI_SIZE_WIDTH_WRAP, MODUI_SIZE_HEIGHT_WRAP);
		this->_side = modui::SIDE_LEFT;
	}

	MemoryText* MemoryText::init(const char* text) { return new MemoryText(text); }

	MemoryText* MemoryText::set_side(modui::Side side)
	{
		this->_side = side;

		return this;
	}

	MemoryText* MemoryText::set_text(const char* text)
	{
		this->_text = text;

		if (modui::get_current_app()->is_rendering())
			this->_update_text_size();

		return this;
	}

	MemoryText* MemoryText::set_font_size(float font_size)
	{
		this->_font_size = font_size;

		if (modui::get_current_app()->is_rendering())
			this->_update_text_size();

		return this;
	}

	void MemoryText::pre_render()
	{
		this->_update_text_size();

		Widget::pre_render();
	}

	void MemoryText::render()
	{
		ImDrawList* draw_list =  ImGui::GetWindowDrawList();
		Theme& theme = this->get_theme();

		if (this->_text == nullptr) return;

		Vec2 text_pos = this->_pos + (this->_calculated_size - this->_text_size) / 2.0f;
		text_pos.x += this->_padding.w;
		text_pos.y += this->_padding.x;

		draw_list->AddText(ImGui::GetFont(), this->_font_size, text_pos, this->is_on_card() ? theme().on_surface_variant : theme().on_surface, this->_text, nullptr, this->_calculated_size.x);
	}

	void MemoryText::_update_text_size(float available_width)
	{
		if ((this->_text == nullptr) || (strlen(this->_text) == 0) || (this->_font_size == 0.0f) || (available_width < 0.0f))
		{
			this->_text_size.x = 0.0f;
			this->_text_size.y = this->_font_size;

			return;
		}

		this->_text_size = ImGui::GetFont()->CalcTextSizeA(this->_font_size, FLT_MAX, available_width, this->_text, nullptr);
		this->_text_size.x += 1.0f;
	}

	Vec2 MemoryText::_calc_side()
	{
		return Vec2(0.0f, 0.0f);
	}

	float MemoryText::get_wrapped_size_x()
	{
		return this->_text_size.x + this->_padding.y + this->_padding.w;
	}

	float MemoryText::get_wrapped_size_y()
	{
		return this->_text_size.y + this->_padding.x + this->_padding.z;
	}

	float MemoryText::calculate_size_x(float bounding_box_size_x)
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

	float MemoryText::calculate_size_y(float bounding_box_size_y)
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

	bool MemoryText::is_on_card()
	{
		return Widget::is_on_card();
	}
}