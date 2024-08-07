#pragma once

#include "../core/common.hpp"
#include "basebutton.hpp"

namespace modui::ui
{
	class Checkbox : public BaseButton
	{
	public:
		Checkbox(bool state);
		static Checkbox* init(bool state = false);

		Checkbox* set_state(bool state) override;
		bool get_state() override;

		Checkbox* on_release(ButtonInputCallback callback) override;

		void render() override;

		float get_wrapped_size_x() override;
		float get_wrapped_size_y() override;

		float calculate_size_x(float bounding_box_size_x) override;
		float calculate_size_y(float bounding_box_size_y) override;

	private:
		float _rounding;

		bool _state;

		float _press_factor;
	};
}