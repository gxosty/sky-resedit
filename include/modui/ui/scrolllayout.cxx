#include <modui/ui/scrolllayout.hpp>

namespace modui::ui
{
	ScrollLayout::ScrollLayout() : Widget()
	{
		this->_size = Vec2(
			MODUI_SIZE_WIDTH_FULL,
			MODUI_SIZE_HEIGHT_FULL
		);
	}

	ScrollLayout* ScrollLayout::init() { return new ScrollLayout(); }

	ScrollLayout* ScrollLayout::add(Widget* widget)
	{
		if (!this->_children.empty())
			throw exceptions::AddWidgetException("Only one widget can be added to `modui::ui::ScrollLayout` object");

		return (ScrollLayout*)Widget::add(widget);
	}

	void ScrollLayout::render()
	{
		if (!this->_children.empty())
		{
			Theme& theme = this->get_theme();

			ImGui::PushID(this->_id);
			ImGui::SetNextWindowPos(this->_pos);

			ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, utils::dp(3));
			ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, FLT_MAX);
			ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, 0);
			ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, theme().secondary);
			ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, theme().secondary);
			ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, theme().secondary);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, 0);
			bool opened = ImGui::BeginChild(DEFAULT_ID, this->_calculated_size, 0,
				  ImGuiWindowFlags_NoTitleBar
				| ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoMove
				| ImGuiWindowFlags_NoCollapse);

			if (opened && !this->_children.empty())
			{
				this->_children[0]->calculate_pos_y(ImGui::GetCursorScreenPos().y);
				this->_children[0]->render();
			}

			ImGui::EndChild();
			ImGui::PopStyleColor(5);
			ImGui::PopStyleVar(2);
			ImGui::PopID();
			// _MODUI_SHOW_BB(this);
		}
	}

	float ScrollLayout::get_wrapped_size_x()
	{
		if (!this->_children.empty())
		{
			return this->_children[0]->get_wrapped_size_x();
		}

		return 0.0f;
	}

	float ScrollLayout::get_wrapped_size_y()
	{
		return 0.0f;
	}

	float ScrollLayout::calculate_pos_x(float bounding_box_pos_x)
	{
		auto ret = Widget::calculate_pos_x(bounding_box_pos_x);

		if (!this->_children.empty())
			this->_children[0]->calculate_pos_x(bounding_box_pos_x);

		return ret;
	}

	float ScrollLayout::calculate_pos_y(float bounding_box_pos_y)
	{
		auto ret = Widget::calculate_pos_y(bounding_box_pos_y);

		// if (!this->_children.empty())
		// 	this->_children[0]->calculate_pos_y(ret);

		return ret;
	}

	float ScrollLayout::calculate_size_x(float bounding_box_size_x)
	{
		float x = this->_size.x;
		this->_bounding_box_size.x = bounding_box_size_x;

		if (x == MODUI_SIZE_WIDTH_FULL)
		{
			x = bounding_box_size_x;
			if (!this->_children.empty()) this->_children[0]->calculate_size_x(x);
		}
		else if (x == MODUI_SIZE_WIDTH_WRAP)
		{
			x = this->get_wrapped_size_x();
		}
		else if (x < 0.0f)
		{
			x = bounding_box_size_x + x;
			if (!this->_children.empty()) this->_children[0]->calculate_size_x(x);
		}

		this->_calculated_size.x = x;

		return x;
	}

	float ScrollLayout::calculate_size_y(float bounding_box_size_y)
	{
		float y = this->_size.y;
		this->_bounding_box_size.y = bounding_box_size_y;

		if ((y == MODUI_SIZE_WIDTH_FULL) || (y == MODUI_SIZE_HEIGHT_WRAP))
		{
			y = bounding_box_size_y;
		}
		else if (y < 0.0f)
		{
			y = bounding_box_size_y + y;
		}

		if (!this->_children.empty()) this->_children[0]->calculate_size_y(99999.0f);

		this->_calculated_size.y = y;

		return y;
	}
}