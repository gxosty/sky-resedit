#include "divider.hpp"

namespace modui::ui
{
	Divider::Divider(Type type) : Widget(),
		_type{type}
	{
		this->_size = {MODUI_SIZE_WIDTH_FULL, utils::dp(1)};
	}

	Divider* Divider::init(Type type) { return new Divider(type); }

	float Divider::get_wrapped_size_x()
	{
		return utils::dp(1);
	}

	float Divider::get_wrapped_size_y()
	{
		return utils::dp(1);
	}

	void Divider::render()
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		Theme& theme = this->get_theme();

		Vec2 size = this->_calculated_size;
		Vec2 pos = this->_pos;

		switch (this->_type)
		{
		case Type::FULL:
			draw_list->AddLine(pos, Vec2(pos.x + size.x, pos.y), theme().outline_variant, utils::dp(1));
			break;
		case Type::INSENT:
			draw_list->AddLine(Vec2(pos.x + utils::dp(16), pos.y), Vec2(pos.x + size.x - utils::dp(16), pos.y), theme().outline_variant, utils::dp(1));
			break;
		}
	}
}