#include "main_screen.hpp"
#include "../resedit_app.hpp"

namespace resedit::ui
{
	MainScreen::MainScreen() : modui::ui::Screen("main_screen") {};

	void MainScreen::add_resource_pack(std::weak_ptr<resedit::core::ResourcePack> resource_pack, size_t index)
	{
		auto rp_holder_ll = this->find_widget_by_id("rp_holder_ll");

		auto rpc = ui::widgets::ResourcePackCard::init(resource_pack, index);

		rpc->find_widget_by_id("pack_delete_ib")
			->on_release(MODUI_CALLBACK(rpc) {
				((ResEditApp*)modui::get_current_app())->remove_resource_pack(rpc);
			});

		rpc->find_widget_by_id("pack_move_down_ib")
			->on_release(MODUI_CALLBACK(rpc) {
				((ResEditApp*)modui::get_current_app())->move_resource_pack(rpc, core::ResourcePackManager::MoveDirection::Down);
			});

		rpc->find_widget_by_id("pack_move_up_ib")
			->on_release(MODUI_CALLBACK(rpc) {
				((ResEditApp*)modui::get_current_app())->move_resource_pack(rpc, core::ResourcePackManager::MoveDirection::Up);
			});

		rp_holder_ll->add(rpc);
	}

	void MainScreen::remove_resource_pack(ui::widgets::ResourcePackCard* resource_pack_card)
	{
		size_t idx = resource_pack_card->get_index();
		auto rp_holder_ll = this->find_widget_by_id("rp_holder_ll");
		rp_holder_ll->remove(resource_pack_card);
		delete resource_pack_card;

		std::vector<modui::ui::Widget*>& ll_children = rp_holder_ll->get_children();

		for (; idx < ll_children.size(); idx++)
		{
			((ui::widgets::ResourcePackCard*)ll_children[idx])->set_index(idx);
		}
	}

	void MainScreen::move_resource_pack(ui::widgets::ResourcePackCard* resource_pack_card, core::ResourcePackManager::MoveDirection direction)
	{
		auto rp_holder_ll = this->find_widget_by_id("rp_holder_ll");
		std::vector<modui::ui::Widget*>& ll_children = rp_holder_ll->get_children();

		for (size_t i = 0; i < ll_children.size(); i++)
		{
			if (ll_children[i] == static_cast<Widget*>(resource_pack_card))
			{
				size_t new_idx = (direction == core::ResourcePackManager::MoveDirection::Up) ? i - 1 : i + 1;
				auto w = ll_children[i];
				ll_children[i] = ll_children[new_idx];
				ll_children[new_idx] = w;

				((ui::widgets::ResourcePackCard*)ll_children[i])->set_index(i);
				resource_pack_card->set_index(new_idx);

				break;
			}
		}
	}

	void MainScreen::on_create_widget()
	{
		using namespace modui::ui;

		this->add(
			LinearLayout::init(LinearLayout::Orientation::VERTICAL)
				->set_size_y(MODUI_SIZE_HEIGHT_FULL)
				->set_padding(DP(10))
				->set_spacing(DP(10))
				->add(
					TitleText::init("ResEdit v" __LIB_VERSION__)
						->set_padding_bottom(DP(5)),

					Text::init("Restart is required for changes to take effect"),

					Button::init("Add Resource Pack")
						->set_size_x(MODUI_SIZE_WIDTH_FULL)
						->on_release(MODUI_CALLBACK(this) {
							resedit::import_resource_pack();
						}),

					ScrollLayout::init()
						->add(
							LinearLayout::init(LinearLayout::Orientation::VERTICAL)
								->set_size_y(MODUI_SIZE_HEIGHT_WRAP)
								->set_spacing(DP(10))
								->set_id("rp_holder_ll")
								
						)
				)
		);
	}
}