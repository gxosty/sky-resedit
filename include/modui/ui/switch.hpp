#pragma once

#include "../core/common.hpp"
#include "basebutton.hpp"

namespace modui::ui
{
	class Switch : public BaseButton
	{
	public:
		Switch(bool state);
		static Switch* init(bool state = false);

		Switch* set_state(bool state) override;
		bool get_state() override;

		Widget* on_release(ButtonInputCallback callback) override;

		void render() override;

		float get_wrapped_size_x() override;
		float get_wrapped_size_y() override;

		float calculate_size_x(float bounding_box_size_x) override;
		float calculate_size_y(float bounding_box_size_y) override;

	private:
		bool _state;

		float _press_factor;
		float _state_factor;
	};
}