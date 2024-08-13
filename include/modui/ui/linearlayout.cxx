#include <modui/ui/linearlayout.hpp>

namespace modui::ui
{
	LinearLayout::LinearLayout(Orientation orientation) : Widget(),
		_orientation{orientation}
	{
		if (orientation == Orientation::VERTICAL)
		{
			this->_size = Vec2(
				MODUI_SIZE_WIDTH_FULL,
				MODUI_SIZE_HEIGHT_WRAP
			);
		}
		else
		{
			this->_size = Vec2(
				MODUI_SIZE_WIDTH_WRAP,
				MODUI_SIZE_HEIGHT_FULL
			);
		}
	};

	LinearLayout* LinearLayout::init(Orientation orientation) { return new LinearLayout(orientation); }

	LinearLayout* LinearLayout::set_orientation(Orientation orientation)
	{
		this->_orientation = orientation;

		return this;
	}

	void LinearLayout::render()
	{
		if (this->_children.empty()) return;

		// ImDrawList* draw_list = ImGui::GetWindowDrawList();
		// draw_list->AddRect(this->_bounding_box_pos, this->_bounding_box_pos + this->_bounding_box_size, 0xff7f7f7f, 0, 0, 2.0);

		for (auto& child : this->_children)
		{
			child->render();
		}

		ImGui::SetCursorScreenPos(this->_pos);
		ImGui::Dummy(this->_calculated_size);

		_MODUI_SHOW_BB(this);
	}

	void LinearLayout::_render_vertical()
	{
		for (auto& child : this->_children)
		{
			child->render();
		}
	}

	void LinearLayout::_render_horizontal()
	{
		for (auto& child : this->_children)
		{
			child->render();
		}
	}

	float LinearLayout::calculate_pos_x(float bounding_box_pos_x)
	{
		auto ret = Widget::calculate_pos_x(bounding_box_pos_x);
		float widget_pos_x = this->_pos.x + this->_padding.w;

		if (this->_orientation == Orientation::HORIZONTAL)
		{
			for (auto child : this->_children)
			{
				widget_pos_x = child->calculate_pos_x(widget_pos_x);
				widget_pos_x += child->get_calculated_size().x + this->_spacing.x;
			}
		}
		else
		{
			for (auto child : this->_children)
			{
				child->calculate_pos_x(widget_pos_x);
			}
		}

		return ret;
	}

	float LinearLayout::calculate_pos_y(float bounding_box_pos_y)
	{
		auto ret = Widget::calculate_pos_y(bounding_box_pos_y);
		float widget_pos_y = this->_pos.y + this->_padding.x;

		if (this->_orientation == Orientation::VERTICAL)
		{
			for (auto child : this->_children)
			{
				widget_pos_y = child->calculate_pos_y(widget_pos_y);
				widget_pos_y += child->get_calculated_size().y + this->_spacing.y;
			}
		}
		else
		{
			for (auto child : this->_children)
			{
				child->calculate_pos_y(widget_pos_y);
			}
		}

		return ret;
	}

	float LinearLayout::calculate_size_x(float bounding_box_size_x)
	{
		this->_bounding_box_size.x = bounding_box_size_x;

		if (this->_orientation == Orientation::HORIZONTAL)
		{
			unsigned widget_count = this->_children.size();
			unsigned widget_full_width_count = 0;
			float inner_width_widgets = 0.0f;
			float widget_full_width = 0.0f;

			for (auto child : this->_children)
			{
				if (child->get_size().x == MODUI_SIZE_WIDTH_FULL)
				{
					widget_full_width_count++;
					continue;
				}
				else if (child->get_size().x == MODUI_SIZE_WIDTH_WRAP)
				{
					inner_width_widgets += child->calculate_size_x(child->get_wrapped_size_x());
				}
				else
				{
					inner_width_widgets += child->calculate_size_x(child->get_size().x);
				}
			}

			if ((this->_size.x == MODUI_SIZE_WIDTH_FULL) 
				|| ((this->_size.x == MODUI_SIZE_WIDTH_WRAP) && widget_full_width_count))
			{
				if (widget_full_width_count)
					widget_full_width = (bounding_box_size_x - this->_padding.y - this->_padding.w - fmax(this->_spacing.x * (widget_count - 1), 0.0f) - inner_width_widgets) / widget_full_width_count;

				this->_calculated_size.x = bounding_box_size_x;
			}
			else
			{
				if (widget_full_width_count)
				{
					widget_full_width = (this->_size.x - this->_padding.y - this->_padding.w - fmax(this->_spacing.x * (widget_count - 1), 0.0f) - inner_width_widgets) / widget_full_width_count;
					this->_calculated_size.x = this->_size.x;
				}
				else if (this->_size.x == MODUI_SIZE_WIDTH_WRAP)
				{
					this->_calculated_size.x = inner_width_widgets + fmax(this->_spacing.x * (widget_count - 1), 0.0f) + this->_padding.y + this->_padding.w;
				}
				else
				{
					this->_calculated_size.x = this->_size.x < 0.0f ? bounding_box_size_x - this->_size.x : this->_size.x;
				}
			}

			if (widget_full_width_count)
			{
				for (auto child : this->_children)
				{
					if (child->get_size().x == MODUI_SIZE_WIDTH_FULL)
						child->calculate_size_x(widget_full_width);
				}
			}
		}
		else
		{
			if (this->_size.x == MODUI_SIZE_WIDTH_FULL)
			{
				this->_calculated_size.x = bounding_box_size_x;

				for (auto child : this->_children)
					child->calculate_size_x(bounding_box_size_x - this->_padding.y - this->_padding.w);
			}
			else if (this->_size.x == MODUI_SIZE_WIDTH_WRAP)
			{
				float max_width = 0.0f;
				float inner_bounding_box_size_x = bounding_box_size_x - this->_padding.y - this->_padding.w;
				unsigned widget_full_width_count = 0;

				for (auto child : this->_children)
				{
					if (child->get_size().x == MODUI_SIZE_WIDTH_FULL)
					{
						widget_full_width_count++;
						continue;
					}

					max_width = fmax(max_width, child->calculate_size_x(inner_bounding_box_size_x));
				}

				if (widget_full_width_count)
				{
					for (auto child : this->_children)
					{
						if (child->get_size().x == MODUI_SIZE_WIDTH_FULL)
							child->calculate_size_x(max_width);
					}
				}

				this->_calculated_size.x = max_width + this->_padding.y + this->_padding.w;
			}
			else
			{
				this->_calculated_size.x = this->_size.x < 0.0f ? bounding_box_size_x + this->_size.x : this->_size.x;

				for (auto child : this->_children)
					child->calculate_size_x(this->_calculated_size.x - this->_padding.y - this->_padding.w);
			}
		}

		return this->_calculated_size.x;
	}

	float LinearLayout::calculate_size_y(float bounding_box_size_y)
	{
		this->_bounding_box_size.y = bounding_box_size_y;

		if (this->_orientation == Orientation::VERTICAL)
		{
			unsigned widget_count = this->_children.size();
			unsigned widget_full_height_count = 0;
			float inner_height_widgets = 0.0f;
			float widget_full_height = 0.0f;

			for (auto child : this->_children)
			{
				if (child->get_size().y == MODUI_SIZE_HEIGHT_FULL)
				{
					widget_full_height_count++;
					continue;
				}
				else if (child->get_size().y == MODUI_SIZE_HEIGHT_WRAP)
				{
					inner_height_widgets += child->calculate_size_y(child->get_wrapped_size_y());
				}
				else
				{
					inner_height_widgets += child->calculate_size_y(child->get_size().y);
				}
			}

			if ((this->_size.y == MODUI_SIZE_HEIGHT_FULL) 
				|| ((this->_size.y == MODUI_SIZE_HEIGHT_WRAP) && widget_full_height_count))
			{
				if (widget_full_height_count)
					widget_full_height = (bounding_box_size_y - this->_padding.x - this->_padding.z - fmax(this->_spacing.y * (widget_count - 1), 0.0f) - inner_height_widgets) / widget_full_height_count;

				this->_calculated_size.y = bounding_box_size_y;
			}
			else
			{
				if (widget_full_height_count)
				{
					widget_full_height = (this->_size.y - this->_padding.x - this->_padding.z - fmax(this->_spacing.y * (widget_count - 1), 0.0f) - inner_height_widgets) / widget_full_height_count;
					this->_calculated_size.y = this->_size.y;
				}
				else if (this->_size.y == MODUI_SIZE_HEIGHT_WRAP)
				{
					this->_calculated_size.y = inner_height_widgets + fmax(this->_spacing.y * (widget_count - 1), 0.0f) + this->_padding.x + this->_padding.z;
				}
				else
				{
					this->_calculated_size.y = this->_size.y < 0.0f ? bounding_box_size_y - this->_size.y : this->_size.y;
				}
			}

			if (widget_full_height_count)
			{
				for (auto child : this->_children)
				{
					if (child->get_size().y == MODUI_SIZE_HEIGHT_FULL)
						child->calculate_size_y(widget_full_height);
				}
			}
		}
		else
		{
			if (this->_size.y == MODUI_SIZE_HEIGHT_FULL)
			{
				this->_calculated_size.y = bounding_box_size_y;

				for (auto child : this->_children)
					child->calculate_size_y(bounding_box_size_y - this->_padding.x - this->_padding.z);
			}
			else if (this->_size.y == MODUI_SIZE_HEIGHT_WRAP)
			{
				float max_height = 0.0f;
				float inner_bounding_box_size_y = bounding_box_size_y - this->_padding.x - this->_padding.z;
				unsigned widget_full_height_count = 0;

				for (auto child : this->_children)
				{
					if (child->get_size().y == MODUI_SIZE_HEIGHT_FULL)
					{
						widget_full_height_count++;
						continue;
					}

					max_height = fmax(max_height, child->calculate_size_y(inner_bounding_box_size_y));
				}

				if (widget_full_height_count)
				{
					for (auto child : this->_children)
					{
						if (child->get_size().y == MODUI_SIZE_HEIGHT_FULL)
							child->calculate_size_y(max_height);
					}
				}

				this->_calculated_size.y = max_height + this->_padding.x + this->_padding.z;
			}
			else
			{
				this->_calculated_size.y = this->_size.y < 0.0f ? bounding_box_size_y + this->_size.y : this->_size.y;

				for (auto child : this->_children)
					child->calculate_size_y(this->_calculated_size.y - this->_padding.x - this->_padding.z);
			}
		}

		return this->_calculated_size.y;
	}
}