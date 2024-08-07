#pragma once

#include "../core/common.hpp"
#include "basebutton.hpp"

#include <string>

namespace modui::ui
{
	class Button : public BaseButton
	{
	public:
		Button(const std::string& text);
		static Button* init(const std::string& text = "");

		Button* set_text(const std::string& text) override;
		const std::string& get_text();

		Button* set_font_size(float font_size) override;

		void pre_render() override;
		void render() override;

		float get_wrapped_size_x() override;
		float get_wrapped_size_y() override;

		float calculate_size_x(float bounding_box_size_x) override;
		float calculate_size_y(float bounding_box_size_y) override;

	private:
		std::string _text;
		float _font_size;
		Vec2 _text_size;
		float _rounding;

		void _update_text_size();

		float _press_factor;
	};
}