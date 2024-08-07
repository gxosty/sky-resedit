#pragma once

#include "../core/common.hpp"
#include "widget.hpp"

namespace modui::ui
{
	class ScrollLayout : public Widget
	{
	public:
		ScrollLayout();

		static ScrollLayout* init();

		ScrollLayout* add(Widget* widget) override;

		void render() override;

		float get_wrapped_size_x() override;
		float get_wrapped_size_y() override;

		float calculate_pos_x(float bounding_box_pos_x) override;
		float calculate_pos_y(float bounding_box_pos_y) override;
		float calculate_size_x(float bounding_box_size_x) override;
		float calculate_size_y(float bounding_box_size_y) override;
	};
}