#include "resedit_app.hpp"
#include "resedit.hpp"

#include "ui/main_screen.hpp"

#include <misc/Logger.h>

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

		modui::Theme theme_dark("theme_dark");

		theme_dark().primary                   = MODUI_COLOR_HEX(0xFF'FFACED);
		theme_dark().on_primary                = MODUI_COLOR_HEX(0xFF'5D0057);
		theme_dark().primary_container         = MODUI_COLOR_HEX(0xFF'D100C5);
		theme_dark().on_primary_container      = MODUI_COLOR_HEX(0xFF'FFFFFF);

		theme_dark().secondary                 = MODUI_COLOR_HEX(0xFF'FFACED);
		theme_dark().on_secondary              = MODUI_COLOR_HEX(0xFF'5D0057);
		theme_dark().secondary_container       = MODUI_COLOR_HEX(0xFF'7C0074);
		theme_dark().on_secondary_container    = MODUI_COLOR_HEX(0xFF'FFC9F0);

		theme_dark().tertiary                  = MODUI_COLOR_HEX(0xFF'FFB3B2);
		theme_dark().on_tertiary               = MODUI_COLOR_HEX(0xFF'680014);
		theme_dark().tertiary_container        = MODUI_COLOR_HEX(0xFF'D83445);
		theme_dark().on_tertiary_container     = MODUI_COLOR_HEX(0xFF'FFFFFF);

		theme_dark().error                     = MODUI_COLOR_HEX(0xFF'FFB4AB);
		theme_dark().on_error                  = MODUI_COLOR_HEX(0xFF'690005);
		theme_dark().error_container           = MODUI_COLOR_HEX(0xFF'93000A);
		theme_dark().on_error_container        = MODUI_COLOR_HEX(0xFF'FFDAD6);

		theme_dark().surface                   = MODUI_COLOR_HEX(0xFF'1C0F19);
		theme_dark().on_surface                = MODUI_COLOR_HEX(0xFF'F5DCEB);
		theme_dark().surface_variant           = MODUI_COLOR_HEX(0xFF'564051);
		theme_dark().on_surface_variant        = MODUI_COLOR_HEX(0xFF'DDBED2);
		theme_dark().surface_container_highest = MODUI_COLOR_HEX(0xFF'40303C);
		theme_dark().surface_container_high    = MODUI_COLOR_HEX(0xFF'352531);
		theme_dark().surface_container         = MODUI_COLOR_HEX(0xFF'291B26);
		theme_dark().surface_container_low     = MODUI_COLOR_HEX(0xFF'251722);
		theme_dark().surface_container_lowest  = MODUI_COLOR_HEX(0xFF'170A14);
		theme_dark().inverse_surface           = MODUI_COLOR_HEX(0xFF'F5DCEB);
		theme_dark().inverse_on_surface        = MODUI_COLOR_HEX(0xFF'3B2C37);
		theme_dark().surface_tint              = MODUI_COLOR_HEX(0xFF'FFACED);

		theme_dark().outline                   = MODUI_COLOR_HEX(0xFF'A4899C);
		theme_dark().outline_variant           = MODUI_COLOR_HEX(0xFF'564051);

		this->get_theme_manager().add(theme_dark);
		this->set_current_theme("theme_dark");
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

	void ResEditApp::move_resource_pack(ui::widgets::ResourcePackCard* resource_pack_card, core::ResourcePackManager::MoveDirection direction)
	{
		if (resedit::move_resource_pack(resource_pack_card->get_resource_pack(), direction))
		{
			LOGI("Moved");
			auto main_screen = (resedit::ui::MainScreen*)_screen_manager->get_screen("main_screen");
			main_screen->move_resource_pack(resource_pack_card, direction);
		}
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