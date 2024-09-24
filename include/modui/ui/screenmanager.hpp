#pragma once

#include "../core/common.hpp"
#include "../animation/screen/screenanimation.hpp"
#include "widget.hpp"
#include "screen.hpp"

#include <string>
#include <unordered_map>

namespace modui::ui
{
	class ScreenManager : public Widget
	{
	public:
		ScreenManager(animation::ScreenAnimation* animation);
		static ScreenManager* init(animation::ScreenAnimation* animation = nullptr);

		ScreenManager* set_screen(const std::string& screen_name) override;
		Screen* get_screen(const std::string& screen_name);

		ScreenManager* add(Widget* screen) override;
		void render() override;

		// float get_wrapped_size_x() override;
		// float get_wrapped_size_y() override;

		float calculate_pos_x(float bounding_box_pos_x) override;
		float calculate_pos_y(float bounding_box_pos_y) override;
		float calculate_size_x(float bounding_box_size_x) override;
		float calculate_size_y(float bounding_box_size_y) override;

	private:
		Screen* _current_screen;
		Screen* _next_screen;

		animation::ScreenAnimation* _animation;
	};
}