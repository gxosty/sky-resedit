#pragma once

#include "../core/common.hpp"
#include "basebutton.hpp"
#include "listitem.hpp"
#include "divider.hpp"

#include <string>

namespace modui::ui
{
	typedef ListItem MenuItem;

	class Menu : public Widget
	{
	public:
		Menu();

		static Menu* init();

		void open(Widget* widget = nullptr);
		void close();
		bool is_open();

		template <class T>
		Menu* add(T widget)
		{
			static_assert(std::is_same<T, MenuItem*>::value || std::is_same<T, Divider*>::value,
				"only MenuItem or Divider can be added");
			Widget::add(widget);
			return this;
		}

		template <class T, typename... Args>
		Menu* add(T widget, Args... args)
		{
			this->add(widget);
			return this->add(args...);
		}

		void render() override;

		float calculate_pos_x(float bounding_box_pos_x) override;
		float calculate_size_x(float bounding_box_size_x) override;
		float calculate_size_y(float bounding_box_size_y) override;

	private:
		static int _open_count;
		bool _is_main;
		bool _should_scroll;

		Widget* _caller_widget;
		bool _open;
		float _state_factor;

		void _on_close();

		void _prepare(ui::Widget* widget);
	};
}