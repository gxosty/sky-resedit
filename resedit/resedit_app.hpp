#pragma once

#include <modui/modui.hpp>
#include "core/resource_pack_manager.hpp"
#include "ui/widgets/resource_pack_card.hpp"

namespace resedit
{
	class ResEditApp : public modui::App
	{
	public:
		ResEditApp();

		modui::ui::Widget* build() override;
		void on_create();
		void resource_pack_imported(std::weak_ptr<core::ResourcePack> resource_pack, size_t index);
		void remove_resource_pack(ui::widgets::ResourcePackCard* resource_pack_card);
		void move_resource_pack(ui::widgets::ResourcePackCard* resource_pack_card, core::ResourcePackManager::MoveDirection direction);

		static void initialize();
		static ResEditApp* get_instance();

	private:
		static ResEditApp* instance;

		modui::ui::ScreenManager* _screen_manager;
		modui::ui::IconButton* _theme_toggler;
	};
}