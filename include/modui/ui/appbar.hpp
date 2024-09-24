#pragma once

#include "../core/common.hpp"
#include "widget.hpp"
#include "iconbutton.hpp"
#include "menu.hpp"

#include <string>
#include <vector>

namespace modui::ui
{
	/* Not full implementation of TopAppBar because meh */
	class TopAppBar : public Widget
	{
	public:
		TopAppBar(
			const std::string& text,
			IconButton* leading_icon_button,
			const std::vector<MenuItem*>& trailing_buttons
		);

		static TopAppBar* init(
			const std::string& text,
			IconButton* leading_icon_button = nullptr,
			const std::vector<MenuItem*>& trailing_buttons = {}
		);

		float calculate_pos_x(float bounding_box_pos_x) override;
		float calculate_pos_y(float bounding_box_pos_y) override;
		float calculate_size_x(float bounding_box_size_x) override;
		float calculate_size_y(float bounding_box_size_y) override;

	private:
		std::string _text;
		IconButton* _leading_icon_button;
		std::vector<MenuItem*> _trailing_buttons;

		Menu* _trailing_menu;
		std::vector<IconButton*> _trailing_visible_buttons;

		float _font_size;
		Vec2 _text_size;

		void _update_trailing_buttons();
		void _update_text_size();
	};
}