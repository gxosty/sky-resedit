#pragma once

#include "../core/common.hpp"
#include "baseslider.hpp"

namespace modui::ui
{
	class Slider : public BaseSlider
	{
	public:
		Slider(float min_value, float max_value);
		static Slider* init(float min_value, float max_value);

		void render() override;

		float get_wrapped_size_x() override;
		float get_wrapped_size_y() override;

		float calculate_size_x(float bounding_box_size_x) override;
		float calculate_size_y(float bounding_box_size_y) override;

	private:
		float _press_factor;
	};
}