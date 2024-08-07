#pragma once

#include "../core/common.hpp"
#include "widget.hpp"

namespace modui::ui
{
	class MemoryText : public Widget
	{
	public:
		MemoryText(const char* text);
		static MemoryText* init(const char* text = nullptr);

		MemoryText* set_side(modui::Side side) override;
		MemoryText* set_text(const char* text) override;
		MemoryText* set_font_size(float font_size) override;

		void pre_render() override;
		void render() override;

		float get_wrapped_size_x() override;
		float get_wrapped_size_y() override;

		float calculate_size_x(float bounding_box_size_x) override;
		float calculate_size_y(float bounding_box_size_y) override;

	private:
		const char* _text;
		float _font_size;
		Vec2 _text_size;
		modui::Side _side;

		void _update_text_size(float available_width = 0.0f);
		Vec2 _calc_side();

		virtual bool is_on_card();
	};
}