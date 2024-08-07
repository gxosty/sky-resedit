#include <modui/ui/listitem.hpp>
#include <modui/app.hpp>
#include <modui/core/image/image.hpp>

#include <cstdio>

namespace modui::ui
{
	ListItem::ListItem(
		const std::string& text,
		const std::string& supporting_text,
		const std::string& trailing_text,
		ImageID leading_icon,
		Widget* trailing_widget,
		Type type
	) : BaseButton(),
		_trailing_widget{trailing_widget},
		_type{type},
		_state{true},
		_press_factor{0.0f},
		_leading_icon_size{0.0f},
		_text_size{0.0f, 0.0f},
		_supporting_text_size{0.0f, 0.0f},
		_trailing_text_size{0.0f, 0.0f},
		_font_size{ImGui::GetFontSize() * 1.15f},
		_supporting_text_font_size{ImGui::GetFontSize()},
		_trailing_text_font_size{ImGui::GetFontSize() * 0.785f}
	{
		this->_size = Vec2(MODUI_SIZE_WIDTH_FULL, MODUI_SIZE_HEIGHT_WRAP);
		this->set_text(text);
		this->set_supporting_text(supporting_text);
		this->set_trailing_text(trailing_text);
		this->set_leading_icon(leading_icon);

		BaseButton::on_release(MODUI_CALLBACK(this) {
			if (this->_trailing_widget)
				this->_trailing_widget->set_state(!this->_trailing_widget->get_state());
		});
	}

	ListItem* ListItem::init(
		const std::string& text,
		const std::string& supporting_text,
		const std::string& trailing_text,
		ImageID leading_icon,
		Widget* trailing_widget,
		Type type	
	) { return new ListItem(text, supporting_text, trailing_text, leading_icon, trailing_widget, type); }

	ListItem* ListItem::set_text(const std::string& text)
	{
		this->_text = text;

		if (modui::get_current_app()->is_rendering())
			this->_update_text_size();

		return this;
	}

	const std::string& ListItem::get_text() const
	{
		return this->_text;
	}

	ListItem* ListItem::set_supporting_text(const std::string& supporting_text)
	{
		this->_supporting_text = supporting_text;

		if (modui::get_current_app()->is_rendering())
			this->_update_supporting_text_size();

		return this;
	}

	const std::string& ListItem::get_supporting_text() const
	{
		return this->_supporting_text;
	}

	ListItem* ListItem::set_trailing_text(const std::string& trailing_text)
	{
		this->_trailing_text = trailing_text;

		if (modui::get_current_app()->is_rendering())
			this->_update_trailing_text_size();

		return this;
	}

	const std::string& ListItem::get_trailing_text() const
	{
		return this->_trailing_text;
	}

	ListItem* ListItem::set_leading_icon(ImageID leading_icon)
	{
		this->_leading_icon = leading_icon;

		if (modui::get_current_app()->is_rendering())
			this->_update_leading_icon_size();

		return this;
	}

	ImageID ListItem::get_leading_icon() const
	{
		return this->_leading_icon;
	}

	ListItem* ListItem::set_trailing_widget(Widget* trailing_widget)
	{
		this->_trailing_widget = trailing_widget;

		if (trailing_widget != nullptr)
			trailing_widget->pre_render();

		return this;
	}

	Widget* ListItem::get_trailing_widget() const
	{
		return this->_trailing_widget;
	}

	ListItem* ListItem::on_release(ButtonInputCallback callback)
	{
		BaseButton::on_release(MODUI_CALLBACK(this, callback) {
			if (this->_trailing_widget)
				this->_trailing_widget->set_state(!this->_trailing_widget->get_state());

			callback(this);
		});

		return this;
	}

	void ListItem::pre_render()
	{
		this->_update_text_size();
		this->_update_supporting_text_size();
		this->_update_trailing_text_size();
		this->_update_leading_icon_size();

		if (this->_trailing_widget != nullptr)
			this->_trailing_widget->pre_render();

		Widget::pre_render();
	}

	void ListItem::render()
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		Theme& theme = this->get_theme();

		Vec2 pos = this->_pos;
		Vec2 size = this->_calculated_size;

		ui::BaseButton::render();

		this->_press_factor = utils::clamp(
			this->_press_factor + (ImGui::GetIO().DeltaTime * MODUI_WIDGET_PRESS_TRANSITION_SPEED) * (this->_is_held ? 1.0f : -1.0f),
			0.0f,
			1.0f
		);

		bool trailing = (!this->_trailing_text.empty() || (this->_trailing_widget != nullptr));

		Vec2 text_pos, supporting_text_pos,
				leading_icon_pos, trailing_pos;

		this->_get_all_pos(text_pos, supporting_text_pos, leading_icon_pos, trailing_pos);

		text_pos += pos;
		supporting_text_pos += pos;
		leading_icon_pos += pos;
		trailing_pos += pos;

		const float text_wrap = trailing ? trailing_pos.x - utils::dp(16) - text_pos.x : 0.0f;
		const float text_clip = trailing ? text_wrap : pos.x + size.x - utils::dp(16) - text_pos.x;

		Col32 container_fill_color = theme().surface;
		Col32 leading_icon_color = (theme().on_surface_variant & 0xFFFFFF);
		Col32 text_color = (theme().on_surface & 0xFFFFFF);
		Col32 supporting_text_color = theme().on_surface_variant;
		Col32 ripple_color = (theme().on_surface & 0xFFFFFF) | (unsigned(0xFF * this->_press_factor * 0.1f) << 24);

		if (this->_state)
		{
			text_color |= unsigned(0xFF) << 24;
			leading_icon_color |= unsigned(0xFF) << 24;
		}
		else
		{
			text_color |= unsigned(0x61) << 24;
			leading_icon_color |= unsigned(0x61) << 24;
		}

		if (this->_leading_icon != nullptr)
		{
			this->_leading_icon->draw(leading_icon_pos, Vec2(_leading_icon_size, _leading_icon_size), leading_icon_color);
		}

		ImVec4 clip = {text_pos.x, text_pos.y, text_pos.x + text_clip, text_pos.y + this->_font_size * (this->_type == Type::THREE_LINED ? 2 : 1)};
		draw_list->AddText(ImGui::GetFont(), this->_font_size, text_pos, text_color, this->_text.c_str(), NULL, text_wrap, &clip);

		if (!this->_supporting_text.empty() && (this->_type != Type::ONE_LINED))
		{
			clip = {supporting_text_pos.x, supporting_text_pos.y, supporting_text_pos.x + text_clip, supporting_text_pos.y + this->_supporting_text_font_size * (this->_type == Type::THREE_LINED ? 2 : 1)};
			draw_list->AddText(ImGui::GetFont(), this->_supporting_text_font_size, supporting_text_pos, supporting_text_color, this->_supporting_text.c_str(), NULL, text_wrap, &clip);
		}

		if (trailing)
		{
			if (this->_trailing_widget != nullptr)
			{
				this->_trailing_widget->calculate_pos_x(trailing_pos.x);
				this->_trailing_widget->calculate_pos_y(trailing_pos.y);
				this->_trailing_widget->render();
			}
			else
			{
				draw_list->AddText(ImGui::GetFont(), this->_trailing_text_font_size, trailing_pos, supporting_text_color, this->_trailing_text.c_str());
			}
		}

		draw_list->AddRectFilled(
			pos,
			pos + size,
			ripple_color
		);
	}

	float ListItem::get_wrapped_size_x()
	{
		bool anything_before = false;
		float x = utils::dp(16) * 2.0f;

		if (this->_leading_icon != nullptr)
		{
			x += this->_leading_icon_size;

			anything_before = true;
		}

		if (!this->_text.empty())
		{
			if (anything_before) x += utils::dp(16);

			x += this->_text_size.x;
			anything_before = true;
		}

		if (!this->_supporting_text.empty())
		{
			if (this->_text.empty() && anything_before) x += utils::dp(16);

			x += fmax(this->_supporting_text_size.x - this->_text_size.x, 0.0f);
			anything_before = true;
		}

		if (this->_trailing_widget != nullptr)
		{
			if (anything_before) x += utils::dp(16);
			x += this->_trailing_widget->get_calculated_size().x; // widget width should be WRAP
		}
		else if (!this->_trailing_text.empty())
		{
			if (anything_before) x += utils::dp(16);
			x += this->_trailing_text_size.x;
		}

		return fmin(x, utils::dp(240));
	}

	float ListItem::get_wrapped_size_y()
	{
		float y = 0.0f;

		switch (this->_type)
		{
		case Type::ONE_LINED:
			y = utils::dp(8) * 2.0f + this->_leading_icon_size;
			y = fmax(y, utils::dp(56));
			break;
		case Type::TWO_LINED:
			y = utils::dp(8) * 2.0f + this->_leading_icon_size;
			y = fmax(y, utils::dp(72));
			break;
		case Type::THREE_LINED:
			y = utils::dp(12) * 2.0f + this->_leading_icon_size;
			y = fmax(y, utils::dp(88));
			break;
		}

		return y;
	}

	float ListItem::calculate_size_x(float bounding_box_size_x)
	{
		float x = this->_size.x;
		this->_bounding_box_size.x = bounding_box_size_x;

		if (x == MODUI_SIZE_WIDTH_FULL)
		{
			x = bounding_box_size_x;

			if (this->_trailing_widget != nullptr)
			{
				this->_trailing_widget->calculate_size_x(this->_trailing_widget->get_wrapped_size_x());
			}
		}
		else if (x == MODUI_SIZE_WIDTH_WRAP)
		{
			if (this->_trailing_widget != nullptr)
			{
				this->_trailing_widget->calculate_size_x(this->_trailing_widget->get_wrapped_size_x());
			}

			x = this->get_wrapped_size_x();
		}
		else if (x < 0.0f)
		{
			x = bounding_box_size_x + x;
			this->_trailing_widget->calculate_size_x(0.0f);

			if (this->_trailing_widget != nullptr)
			{
				this->_trailing_widget->calculate_size_x(0.0f);
			}
		}

		this->_calculated_size.x = x;

		return x;
	}

	float ListItem::calculate_size_y(float bounding_box_size_y)
	{
		float y = this->_size.y;
		this->_bounding_box_size.y = bounding_box_size_y;

		if (y == MODUI_SIZE_WIDTH_FULL)
		{
			y = bounding_box_size_y;
		}
		else if (y == MODUI_SIZE_HEIGHT_WRAP)
		{
			y = this->get_wrapped_size_y();
		}
		else if (y < 0.0f)
		{
			y = bounding_box_size_y + y;
		}

		if (this->_trailing_widget)
			this->_trailing_widget->calculate_size_y(this->_trailing_widget->get_wrapped_size_y());

		this->_calculated_size.y = y;

		return y;
	}

	void ListItem::_update_text_size()
	{
		if (this->_text.empty() || this->_font_size == 0.0f)
		{
			this->_text_size.x = 0.0f;
			this->_text_size.y = 0.0f;

			return;
		}

		this->_text_size = ImGui::GetFont()->CalcTextSizeA(this->_font_size, FLT_MAX, 0.0f, this->_text.c_str(), nullptr);
	}

	void ListItem::_update_supporting_text_size()
	{
		if (this->_supporting_text.empty() || this->_font_size == 0.0f)
		{
			this->_supporting_text_size.x = 0.0f;
			this->_supporting_text_size.y = 0.0f;

			return;
		}

		this->_supporting_text_size = ImGui::GetFont()->CalcTextSizeA(this->_supporting_text_font_size, FLT_MAX, 0.0f, this->_supporting_text.c_str(), nullptr);
	}

	void ListItem::_update_trailing_text_size()
	{
		if (this->_trailing_text.empty() || this->_font_size == 0.0f)
		{
			this->_trailing_text_size.x = 0.0f;
			this->_trailing_text_size.y = 0.0f;

			return;
		}

		this->_trailing_text_size = ImGui::GetFont()->CalcTextSizeA(this->_trailing_text_font_size, FLT_MAX, 0.0f, this->_trailing_text.c_str(), nullptr);
	}

	void ListItem::_update_leading_icon_size()
	{
		if (this->_leading_icon == nullptr)
		{
			this->_leading_icon_size = 0.0f;
			return;
		}

		switch (this->_leading_icon->get_image_type())
		{
		case modui::image::ImageType::RASTER_IMAGE:
		case modui::image::ImageType::VECTOR_IMAGE:
			this->_leading_icon_size = utils::dp(56);
			break;
		case modui::image::ImageType::ICON:
			this->_leading_icon_size = utils::dp(24);
			break;
		case modui::image::ImageType::AVATAR:
			this->_leading_icon_size = utils::dp(40);
			break;
		default:
			this->_leading_icon_size = 0.0f;
			break;
		}
	}

	void ListItem::_get_all_pos(
		Vec2& text_pos,
		Vec2& supporting_text_pos,
		Vec2& leading_icon_pos,
		Vec2& trailing_pos)
	{
		Vec2 size = this->_calculated_size;
		float trailing_height = 0.0f;

		text_pos = {utils::dp(16), 0.0f};
		supporting_text_pos = {utils::dp(16), 0.0f};
		leading_icon_pos = {utils::dp(16), 0.0f};
		trailing_pos = {0.0f, 0.0f};

		if (this->_leading_icon != nullptr)
		{
			text_pos.x += this->_leading_icon_size + utils::dp(16);
			supporting_text_pos.x += this->_leading_icon_size + utils::dp(16);
		}

		if (this->_trailing_widget != nullptr)
		{
			trailing_pos.x = size.x - utils::dp(16) - this->_trailing_widget->get_calculated_size().x;
			trailing_height = this->_trailing_widget->get_calculated_size().y;
		}
		else if (!this->_trailing_text.empty())
		{
			trailing_pos.x = size.x - utils::dp(16) - this->_trailing_text_size.x;
			trailing_height = this->_trailing_text_font_size;
		}

		switch (this->_type)
		{
		case Type::ONE_LINED:
			text_pos.y = (size.y - this->_font_size) / 2.0f;
			leading_icon_pos.y = (size.y - this->_leading_icon_size) / 2.0f;
			trailing_pos.y = (size.y - trailing_height) / 2.0f;
			break;

		case Type::TWO_LINED:
			if (this->_supporting_text.empty())
			{
				text_pos.y = (size.y - this->_font_size) / 2.0f;
			}
			else
			{
				const float t_y = (size.y - (this->_font_size + this->_supporting_text_font_size)) / 2.0f;
				text_pos.y = t_y;
				supporting_text_pos.y = t_y + this->_font_size;
			}

			leading_icon_pos.y = (size.y - this->_leading_icon_size) / 2.0f;
			trailing_pos.y = (size.y - trailing_height) / 2.0f;
			break;

		case Type::THREE_LINED:
			if (this->_supporting_text.empty())
			{
				text_pos.y = utils::dp(12);
			}
			else
			{
				const float t_y = utils::dp(12);
				text_pos.y = t_y;
				supporting_text_pos.y = t_y + this->_font_size;
			}

			leading_icon_pos.y = utils::dp(12);
			trailing_pos.y = utils::dp(12);

			break;
		}
	}
}