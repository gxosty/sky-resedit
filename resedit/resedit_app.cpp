#include "resedit_app.hpp"

#include "ui/main_screen.hpp"

namespace resedit
{
	ResEditApp* ResEditApp::instance = nullptr;

	ResEditApp::ResEditApp() : modui::App("ResEdit v" __LIB_VERSION__)
	{
		this->set_window_fullscreen(false);
		this->set_window_closable(true);
		this->set_window_collapsable(true);

		this->set_window_size(
			Vec2(
				modui::utils::dp_raw(300),
				modui::utils::dp_raw(350)
			)
		);
	}

	modui::ui::Widget* ResEditApp::build()
	{
		auto main_screen = resedit::ui::MainScreen::init();

		_screen_manager = modui::ui::ScreenManager::init()
			->add(main_screen);

		return _screen_manager;
	}

	void ResEditApp::initialize()
	{
		if (!ResEditApp::instance)
			ResEditApp::instance = new ResEditApp();
	}

	ResEditApp* ResEditApp::get_instance()
	{
		return ResEditApp::instance;
	}
}