#pragma once

#include <modui/modui.hpp>
#include "../../core/resource_pack.hpp"

#include <memory>

namespace resedit::ui::widgets
{
	class ResourcePackCard : public modui::ui::FilledCard
	{
	public:
		ResourcePackCard(std::weak_ptr<resedit::core::ResourcePack> resource_pack, size_t index);

		static ResourcePackCard* init(std::weak_ptr<resedit::core::ResourcePack> resource_pack, size_t index);

		std::weak_ptr<resedit::core::ResourcePack> get_resource_pack();

		void set_index(size_t index);
		size_t get_index();

	private:
		void on_create_widget() override;

		void update_priority_text();

	private:
		std::weak_ptr<resedit::core::ResourcePack> _resource_pack;
		size_t _index;
	};
}