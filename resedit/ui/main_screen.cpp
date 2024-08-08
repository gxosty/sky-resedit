#include "main_screen.hpp"
#include "../resedit.hpp"

namespace resedit::ui
{
	MainScreen::MainScreen() : modui::ui::Screen("main_screen") {};

	void MainScreen::on_create_widget()
	{
		using namespace modui::ui;

		this->add(
			Button::init("Button")
		);
	}
}