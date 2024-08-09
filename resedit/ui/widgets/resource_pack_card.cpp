#include "resource_pack_card.hpp"
#include "../../resedit_app.hpp"

namespace resedit::ui::widgets
{
	ResourcePackCard::ResourcePackCard(std::weak_ptr<resedit::core::ResourcePack> resource_pack, size_t index)
		: modui::ui::FilledCard(modui::ui::LinearLayout::Orientation::VERTICAL),
		_resource_pack{resource_pack},
		_index{index} {}

	ResourcePackCard* ResourcePackCard::init(std::weak_ptr<resedit::core::ResourcePack> resource_pack, size_t index)
	{
		return MODUI_WIDGET_INIT(ResourcePackCard, resource_pack, index);
	}

	std::weak_ptr<resedit::core::ResourcePack> ResourcePackCard::get_resource_pack()
	{
		return _resource_pack;
	}

	size_t ResourcePackCard::get_index()
	{
		return _index;
	}

	void ResourcePackCard::on_create_widget()
	{
		using namespace modui::ui;

		std::string name{"Unknown"};
		std::string author{"Unknown"};

		if (auto resource_pack = _resource_pack.lock())
		{
			name = resource_pack->get_name();
			author = resource_pack->get_author();
		}

		this->set_padding(DP(10));
		this->set_spacing(DP(10));
		this->set_size_x(MODUI_SIZE_WIDTH_FULL);
		this->set_size_y(MODUI_SIZE_HEIGHT_WRAP);
		this->add(
			LinearLayout::init(LinearLayout::Orientation::VERTICAL)
				->set_size_x(MODUI_SIZE_WIDTH_FULL)
				->set_size_y(MODUI_SIZE_HEIGHT_WRAP)
				->add(
					TitleText::init(name)
						->set_id("pack_name_tt"),

					Text::init(author)
						->set_id("pack_author_t")
				),

			LinearLayout::init(LinearLayout::Orientation::HORIZONTAL)
				->set_size_x(MODUI_SIZE_WIDTH_FULL)
				->set_size_y(MODUI_SIZE_HEIGHT_WRAP)
				->set_spacing(DP(10))
				->add(
					Widget::init(),

					OutlinedIconButton::init(modui::icons::ic_delete_outline)
						->set_id("pack_delete_ib")
						->on_release(MODUI_CALLBACK(this) {
							((ResEditApp*)modui::get_current_app())->remove_resource_pack(this);
						}),

					FilledIconButton::init(modui::icons::ic_arrow_down)
						->set_id("pack_move_down_ib"),

					FilledIconButton::init(modui::icons::ic_arrow_up)
						->set_id("pack_move_up_ib")
				)
		);
	}
}