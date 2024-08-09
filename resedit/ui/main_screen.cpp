#include "main_screen.hpp"

namespace resedit::ui
{
	MainScreen::MainScreen() : modui::ui::Screen("main_screen") {};

	void MainScreen::add_resource_pack(std::weak_ptr<resedit::core::ResourcePack> resource_pack, size_t index)
	{
		auto rp_holder_ll = this->find_widget_by_id("rp_holder_ll");

		rp_holder_ll->add(
			ui::widgets::ResourcePackCard::init(resource_pack, index)
		);
	}

	void MainScreen::remove_resource_pack(ui::widgets::ResourcePackCard* resource_pack_card)
	{
		auto rp_holder_ll = this->find_widget_by_id("rp_holder_ll");

		rp_holder_ll->remove(resource_pack_card);
	}

	void MainScreen::on_create_widget()
	{
		using namespace modui::ui;

		this->add(
			ScrollLayout::init()
				->add(
					LinearLayout::init(LinearLayout::Orientation::VERTICAL)
						->set_size_y(MODUI_SIZE_HEIGHT_WRAP)
						->set_padding(DP(10))
						->set_spacing(DP(10))
						->add(
							TitleText::init("ResEdit v" __LIB_VERSION__)
								->set_padding_bottom(DP(5)),

							Text::init("Restart is required for resource packs to take effect"),

							Button::init("Add Resource Pack")
								->set_size_x(MODUI_SIZE_WIDTH_FULL)
								->on_release(MODUI_CALLBACK(this) {
									resedit::import_resource_pack();
								}),

							LinearLayout::init(LinearLayout::Orientation::VERTICAL)
								->set_size_y(MODUI_SIZE_HEIGHT_WRAP)
								->set_spacing(DP(10))
								->set_id("rp_holder_ll")
						)
				)
		);
	}
}