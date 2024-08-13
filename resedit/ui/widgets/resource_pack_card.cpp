#include "resource_pack_card.hpp"
#include "../../resedit_app.hpp"
#include "../../resedit.hpp"

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

	void ResourcePackCard::set_index(size_t index)
	{
		_index = index;
		this->update_priority_text();
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
		std::string description{"Unknown"};
		std::string version{"Unknown"};
		int sky_version{0};
		bool enabled = false;

		if (auto resource_pack = _resource_pack.lock())
		{
			name = resource_pack->get_name();
			author = resource_pack->get_author();
			description = resource_pack->get_description();
			version = resource_pack->get_version();
			sky_version = resource_pack->get_sky_version();
			enabled = resource_pack->is_enabled();
		}

		this->set_size_x(MODUI_SIZE_WIDTH_FULL);
		this->set_size_y(MODUI_SIZE_HEIGHT_WRAP);
		this->add(
			LinearLayout::init(LinearLayout::Orientation::HORIZONTAL)
				->set_size_x(MODUI_SIZE_WIDTH_FULL)
				->set_size_y(MODUI_SIZE_HEIGHT_WRAP)
				->set_padding(DP(10))
				->add(
					LinearLayout::init(LinearLayout::Orientation::VERTICAL)
						->set_size_x(MODUI_SIZE_WIDTH_FULL)
						->set_size_y(MODUI_SIZE_HEIGHT_WRAP)
						->add(
							TitleText::init(name),

							Text::init(version)
						),

					Switch::init(enabled)
						->on_release(MODUI_CALLBACK(this) {
							bool enabled = this_widget->get_state();
							resedit::set_resource_pack_enabled(this->get_resource_pack(), enabled);
						})
				),

			LinearLayout::init(LinearLayout::Orientation::VERTICAL)
				->set_padding(DP(10))
				->set_spacing(DP(5))
				->add(
					Text::init(description),

					Text::init("Supported Sky build: " + (sky_version == 0 ? "Any" : std::to_string(sky_version)))
				),

			Divider::init(Divider::Type::INSENT),

			LinearLayout::init(LinearLayout::Orientation::HORIZONTAL)
				->set_size_x(MODUI_SIZE_WIDTH_FULL)
				->set_size_y(MODUI_SIZE_HEIGHT_WRAP)
				->set_padding(DP(10))
				->add(
					Text::init("Author: " + author),

					Widget::init(),

					LinearLayout::init(LinearLayout::Orientation::VERTICAL)
						->set_size_x(MODUI_SIZE_WIDTH_WRAP)
						->set_spacing(DP(5))
						->add(
							Text::init()->set_id("pack_priority_t"),

							LinearLayout::init(LinearLayout::Orientation::HORIZONTAL)
								->set_size_y(MODUI_SIZE_HEIGHT_WRAP)
								->set_spacing(DP(10))
								->add(
									OutlinedIconButton::init(modui::icons::ic_delete_outline)
										->set_id("pack_delete_ib"),

									FilledIconButton::init(modui::icons::ic_arrow_down)
										->set_id("pack_move_down_ib"),

									FilledIconButton::init(modui::icons::ic_arrow_up)
										->set_id("pack_move_up_ib")
								)
						)
				)
		);

		this->update_priority_text();
	}

	void ResourcePackCard::update_priority_text()
	{
		this->find_widget_by_id("pack_priority_t")->set_text("Priority #" + std::to_string(_index + 1));
	}
}