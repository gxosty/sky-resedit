#pragma once

#include "../resedit.hpp"
#include "widgets/resource_pack_card.hpp"

#include <modui/modui.hpp>

namespace resedit::ui
{
	class MainScreen : public modui::ui::Screen
	{
	public:
		MainScreen();

		static MainScreen* init() { return MODUI_WIDGET_INIT(MainScreen); }

		void add_resource_pack(std::weak_ptr<resedit::core::ResourcePack> resource_pack, size_t index);
		void remove_resource_pack(ui::widgets::ResourcePackCard* resource_pack_card);

	private:
		void on_create_widget() override;
	};
}