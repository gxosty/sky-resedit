#pragma once

#include "../core/common.hpp"
#include "widget.hpp"

#include <string>

namespace modui::ui
{
	class Screen : public Widget
	{
	public:
		Screen(const std::string& name);
		static Screen* init(const std::string& name = "");

		Screen* set_name(const std::string& name) override;
		const std::string& get_name();

		Screen* add(Widget* widget) override;
		void render() override;

		// float get_wrapped_size_x() override;
		// float get_wrapped_size_y() override;

		float calculate_pos_x(float bounding_box_pos_x) override;
		float calculate_pos_y(float bounding_box_pos_y) override;
		float calculate_size_x(float bounding_box_size_x) override;
		float calculate_size_y(float bounding_box_size_y) override;

	public:
		float alpha;

	private:
		std::string _name;
	};
}