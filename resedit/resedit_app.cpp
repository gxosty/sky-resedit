#include "resedit_app.hpp"
#include "resedit.hpp"

#include "ui/main_screen.hpp"

namespace resedit
{
	ResEditApp* ResEditApp::instance = nullptr;

	ResEditApp::ResEditApp() : modui::App("ResEdit")
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

		this->on_create();
		return _screen_manager;
	}

	void ResEditApp::on_create()
	{
		auto main_screen = (resedit::ui::MainScreen*)_screen_manager->get_screen("main_screen");

		for (size_t i = 0; i < resedit::get_resource_pack_count(); i++)
		{
			auto rp = resedit::get_resource_pack(i);
			main_screen->add_resource_pack(rp, i);
		}

		resedit::set_import_callback([](std::weak_ptr<core::ResourcePack> resource_pack, ImportResult import_result)
		{
			if (import_result == resedit::ImportResult::Imported)
			{
				ResEditApp::get_instance()->run_in_ui_thread([]() -> void {
					size_t idx = resedit::get_resource_pack_count() - 1;
					auto rp = resedit::get_resource_pack(idx);
					ResEditApp::get_instance()->resource_pack_imported(rp, idx);
				});
			}
		});
	}

	void ResEditApp::resource_pack_imported(std::weak_ptr<core::ResourcePack> resource_pack, size_t index)
	{
		auto main_screen = (resedit::ui::MainScreen*)_screen_manager->get_screen("main_screen");
		main_screen->add_resource_pack(resource_pack, index);
	}

	void ResEditApp::remove_resource_pack(ui::widgets::ResourcePackCard* resource_pack_card)
	{
		auto main_screen = (resedit::ui::MainScreen*)_screen_manager->get_screen("main_screen");

		resedit::remove_resource_pack(resource_pack_card->get_resource_pack());

		main_screen->remove_resource_pack(resource_pack_card);
	}

	void ResEditApp::initialize()
	{
		if (!ResEditApp::instance)
		{
			ResEditApp::instance = new ResEditApp();
		}
	}

	ResEditApp* ResEditApp::get_instance()
	{
		return ResEditApp::instance;
	}
}