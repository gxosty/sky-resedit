#pragma once

#include "../core/common.hpp"
#include "widget.hpp"

namespace modui::ui
{
	class LinearLayout : public Widget
	{
	public:
		enum class Orientation
		{
			VERTICAL = 0,
			HORIZONTAL
		};

	public:
		LinearLayout(Orientation orientation);

		static LinearLayout* init(
			Orientation orientation = Orientation::VERTICAL
		);

		void render() override;

		LinearLayout* set_orientation(Orientation orientation);

		float calculate_pos_x(float bounding_box_pos_x) override;
		float calculate_pos_y(float bounding_box_pos_y) override;
		float calculate_size_x(float bounding_box_size_x) override;
		float calculate_size_y(float bounding_box_size_y) override;

	private:
		Orientation _orientation;

		void _render_vertical();
		void _render_horizontal();
	};
}