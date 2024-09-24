#include <modui/ui/menu.hpp>
#include <modui/ui/basebutton.hpp>
#include <modui/app.hpp>

#include <cstdio>

namespace modui::ui
{
	int Menu::_open_count = 0;

	Menu::Menu()
	{
		this->_open = false;
		this->_is_main = false;
		this->_state_factor = 0.0f;
		this->_should_scroll = false;
	}

	Menu* Menu::init() { return new Menu(); }

	void Menu::open(Widget* widget)
	{
		if (this->_open) return;

		this->_caller_widget = widget;
		this->_open = true;

		if (!Menu::_open_count++)
		{
			this->_is_main = true;
		}

		this->_should_scroll = false;
		this->_prepare(widget);

		modui::get_current_app()->add_foreground_widget(this);
	}

	void Menu::close()
	{
		this->_open = false;
	}

	void Menu::_on_close()
	{
		Menu::_open_count--;
		this->_is_main = false;
		modui::get_current_app()->remove_foreground_widget(this);
	}

	bool Menu::is_open()
	{
		return this->_open;
	}

	void Menu::render()
	{
		Theme& theme = this->get_theme();

		Vec2 size = this->_calculated_size;
		Vec2 pos = this->_pos;

		this->_state_factor = utils::clamp(
			this->_state_factor + (ImGui::GetIO().DeltaTime * MODUI_WIDGET_PRESS_TRANSITION_SPEED) * (this->_open ? 1.0f : -1.0f),
			0.0f,
			1.0f
		);

		float bkp_theme_alpha = Theme::global_alpha;
		Theme::global_alpha = this->_state_factor;

		ImGui::PushID(this->_id);
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		ImGui::Begin(DEFAULT_ID, nullptr,
			  ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoScrollbar);

		pos = Vec2(pos.x, pos.y - ImGui::GetCurrentWindow()->Scroll.y);
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		// draw_list->PushClipRect(this->_pos, this->_pos + (this->_calculated_size * utils::map(this->_state_factor, 0.0f, 1.0f, 0.9f, 1.0f)));
		draw_list->PushClipRect(this->_pos, this->_pos + this->_calculated_size);
		draw_list->PushClipRectFullScreen();

		if (this->_is_main)
		{
			// draw_list->PushClipRectFullScreen();
			// draw_list->AddRectFilled(
			// 	Vec2(0.0f, 0.0f),
			// 	ImGui::GetIO().DisplaySize,
			// 	theme(ThemeColor::OnSurface)
			// );
			// auto bkp_cursor = ImGui::GetCursorScreenPos();
			// ImGui::SetCursorScreenPos({0.0f, 0.0f});
			// ImGui::Dummy(ImGui::GetIO().DisplaySize);
			// ImGui::SetCursorScreenPos(bkp_cursor);
			// draw_list->PopClipRect();
		}

		draw_list->AddRectFilled(
			pos,
			pos + size,
			theme(ThemeColor::SurfaceContainer),
			utils::dp(4)
		);

		bool any_clicked = false;
		float widget_pos_y = pos.y + utils::dp(8);
		for (auto child : this->_children)
		{
			widget_pos_y = child->calculate_pos_y(widget_pos_y);
			widget_pos_y += child->get_calculated_size().y;
			child->render();
			if (ImGui::IsItemDeactivated())
			{
				any_clicked = true;
			}
		}

		ImVec2 mouse_pos = ImGui::GetMousePos();
		bool not_in_window = (
			((mouse_pos.x < this->_pos.x) || (mouse_pos.x > (this->_pos.x + this->_calculated_size.x))) ||
			((mouse_pos.y < this->_pos.y) || (mouse_pos.y > (this->_pos.y + this->_calculated_size.y)))
		);

		if ((any_clicked && !not_in_window) || (!any_clicked && ImGui::IsMouseReleased(0) && not_in_window))
		{
			this->close();
		}

		Theme::global_alpha = bkp_theme_alpha;

		draw_list->PopClipRect();
		ImGui::End();
		ImGui::PopID();

		if (this->_state_factor == 0.0f) this->_on_close();
	}

	float Menu::calculate_pos_x(float bounding_box_pos_x)
	{
		auto ret = Widget::calculate_pos_x(bounding_box_pos_x);

		for (auto child : this->_children)
		{
			child->calculate_pos_x(this->_pos.x);
		}

		return ret;
	}

	float Menu::calculate_size_x(float bounding_box_size_x)
	{
		this->_bounding_box_size.x = bounding_box_size_x;
		float max_width = 0.0f;

		for (auto child : this->_children)
		{
			child->set_spacing(utils::dp(12));
			float child_width = child->get_wrapped_size_x();
			max_width = fmax(max_width, child_width);
		}

		max_width = utils::clamp(max_width, utils::dp(112), utils::dp(280));
		this->_calculated_size.x = max_width;

		for (auto child : this->_children)
		{
			child->calculate_size_x(max_width);
		}

		return max_width;
	}

	float Menu::calculate_size_y(float bounding_box_size_y)
	{
		this->_bounding_box_size.y = bounding_box_size_y;

		float max_height = 0.0f;
		float height = utils::dp(48);
		float divider_height = utils::dp(16);

		for (auto child : this->_children)
		{

			if (dynamic_cast<ui::Divider*>(child))
			{
				child->set_size_y(divider_height);
				max_height += child->calculate_size_y(divider_height);
			}
			else
			{
				child->set_size_y(height);
				max_height += child->calculate_size_y(height);
			}
		}

		this->_calculated_size.y = max_height + utils::dp(16); /* DP(8) * 2 */

		return this->_calculated_size.y;
	}

	void Menu::_prepare(ui::Widget* widget)
	{
		if (!widget)
		{
			this->_pos = ImGui::GetMousePos();
			return;
		}

		if (dynamic_cast<ui::MenuItem*>(widget) && (Menu::_open_count > 1))
		{
			Vec2 pos{0.0f, 0.0f};
			Vec2 size{
				this->calculate_size_x(0.0f),
				this->calculate_size_y(0.0f)
			};

			Vec2 widget_pos = widget->get_pos();
			Vec2 widget_size = widget->get_calculated_size();
			Vec2 display_size = ImGui::GetIO().DisplaySize;

			if ((widget_pos.x + widget_size.x + size.x) < display_size.x)
			{
				pos.x = widget_pos.x + widget_size.x;
			}
			else
			{
				pos.x = widget_pos.x - size.x;
			}

			if (size.y > display_size.y)
			{
				this->_should_scroll = true;
				pos.y = 0.0f;
			}
			else if ((widget_pos.y + widget_size.y + size.y) > display_size.y)
			{
				pos.y = widget_pos.y - size.y;
			}
			else
			{
				pos.y = widget_pos.y + widget_size.y;
			}

			this->_pos = pos;
			return;
		}
		else if (dynamic_cast<ui::BaseButton*>(widget))
		{
			Vec2 pos{0.0f, 0.0f};
			Vec2 size{
				this->calculate_size_x(0.0f),
				this->calculate_size_y(0.0f)
			};

			Vec2 widget_center = widget->get_pos() + (widget->get_calculated_size() / 2.0f);
			Vec2 window_center = modui::get_current_app()->get_window_pos() + modui::get_current_app()->get_window_size() / 2.0f;
			Vec2 widget_pos = widget->get_pos();
			Vec2 widget_size = widget->get_calculated_size();
			Vec2 display_size = ImGui::GetIO().DisplaySize;

			if (widget_center.x > window_center.x)
			{
				pos.x = widget_pos.x + widget_size.x - size.x;
			}
			else
			{
				pos.x = widget_pos.x;
			}

			if (size.y > display_size.y)
			{
				this->_should_scroll = true;
				pos.y = 0.0f;
			}
			else if ((widget_pos.y + widget_size.y + size.y) > display_size.y)
			{
				pos.y = widget_pos.y - size.y;
			}
			else
			{
				pos.y = widget_pos.y + widget_size.y;
			}

			this->_pos = pos;
			return;
		}

		this->_pos = ImGui::GetMousePos();
	}
}