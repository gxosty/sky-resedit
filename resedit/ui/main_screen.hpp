#pragma once

#include <modui/modui.hpp>

namespace resedit::ui
{
	class MainScreen : public modui::ui::Screen
	{
	public:
		MainScreen();

		static MainScreen* init() { return MODUI_WIDGET_INIT(MainScreen); }

	private:
		void on_create_widget() override;
	};
}