#pragma once

#include "../core/common.hpp"
#include "widget.hpp"

#include <string>

namespace modui::ui
{
	class Text : public Widget
	{
	public:
		Text(const std::string& text);
		static Text* init(const std::string& text = "");

		Text* set_side(modui::Side side) override;

		Text* set_text(const std::string& text) override;
		Text* set_font_size(float font_size) override;

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
		modui::Side _side;

		void _update_text_size(float available_width = 0.0f);

		Vec2 _calc_side();

		virtual bool is_on_card();
	};

	class TitleText : public Text
	{
	public:
		TitleText(const std::string& text);
		static TitleText* init(const std::string& text = "");

	private:
		bool is_on_card() override;
	};
}