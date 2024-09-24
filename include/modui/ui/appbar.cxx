#include <modui/ui/appbar.hpp>

namespace modui::icons
{
	char ic_more_vert_bfr[] = R"=(<svg xmlns="http://www.w3.org/2000/svg" height="24px" viewBox="0 -960 960 960" width="24px" fill="#e8eaed"><path d="M480-160q-33 0-56.5-23.5T400-240q0-33 23.5-56.5T480-320q33 0 56.5 23.5T560-240q0 33-23.5 56.5T480-160Zm0-240q-33 0-56.5-23.5T400-480q0-33 23.5-56.5T480-560q33 0 56.5 23.5T560-480q0 33-23.5 56.5T480-400Zm0-240q-33 0-56.5-23.5T400-720q0-33 23.5-56.5T480-800q33 0 56.5 23.5T560-720q0 33-23.5 56.5T480-640Z"/></svg>)=""\x00";
	ImageID ic_more_vert = modui::image::Image::load_from_memory(ic_more_vert_bfr, -1, modui::image::ImageType::ICON);
}

namespace modui::ui
{
	TopAppBar::TopAppBar(
		const std::string& text,
		IconButton* leading_icon_button,
		const std::vector<MenuItem*>& trailing_buttons
	) : Widget(),
		_text{text},
		_leading_icon_button{leading_icon_button},
		_trailing_buttons{trailing_buttons},
		_trailing_menu{nullptr}
	{
		this->_size = Vec2(
			MODUI_SIZE_WIDTH_FULL,
			utils::dp(64)
		);
		this->_update_trailing_buttons();
	}

	TopAppBar* TopAppBar::init(
		const std::string& text,
		IconButton* leading_icon_button,
		const std::vector<MenuItem*>& trailing_buttons
	) { return new TopAppBar(text, leading_icon_button, trailing_buttons); }

	float TopAppBar::calculate_size_x(float bounding_box_size_x)
	{
		float x = this->_size.x;

		if (x == MODUI_SIZE_WIDTH_FULL)
		{
			x = bounding_box_size_x;
		}
		else if (x == MODUI_SIZE_WIDTH_WRAP)
		{
			x = 0.0f;
		}
		else if (x < 0.0f)
		{
			x = bounding_box_size_x + x;
		}

		this->_bounding_box_size.x = bounding_box_size_x;
		this->_calculated_size.x = x;

		if (this->_leading_icon_button)
			this->_leading_icon_button->calculate_size_x(utils::dp(40));

		for (auto widget : this->_trailing_visible_buttons)
		{
			widget->calculate_size_x(utils::dp(40));
		}

		return x;
	}

	float TopAppBar::calculate_size_y(float bounding_box_size_y)
	{
		float y = this->_size.y;

		if (y == MODUI_SIZE_WIDTH_FULL)
		{
			y = bounding_box_size_y;
		}
		else if (y == MODUI_SIZE_HEIGHT_WRAP)
		{
			y = 0.0f;
		}
		else if (y < 0.0f)
		{
			y = bounding_box_size_y + y;
		}

		this->_bounding_box_size.y = bounding_box_size_y;
		this->_calculated_size.y = y;

		if (this->_leading_icon_button)
			this->_leading_icon_button->calculate_size_y(utils::dp(40));

		for (auto widget : this->_trailing_visible_buttons)
		{
			widget->calculate_size_y(utils::dp(40));
		}

		return y;
	}

	float TopAppBar::calculate_pos_x(float bounding_box_pos_x)
	{
		this->_pos.x = bounding_box_pos_x;
		this->_bounding_box_pos.x = bounding_box_pos_x;

		return this->_pos.x;
	}

	float TopAppBar::calculate_pos_y(float bounding_box_pos_y)
	{
		this->_pos.y = bounding_box_pos_y;
		this->_bounding_box_pos.y = bounding_box_pos_y;

		return this->_pos.y;
	}

	void TopAppBar::_update_trailing_buttons()
	{
		for (auto ib : this->_trailing_visible_buttons)
		{
			delete ib;
		}

		this->_trailing_visible_buttons.clear();

		for (int i = this->_trailing_menu->get_children().size(); (i >= 0) && !this->_trailing_menu->get_children().empty(); i--)
		{
			this->_trailing_menu->remove(this->_trailing_menu->get_children()[i]);
		}

		for (int i = 0; i < this->_trailing_buttons.size(); i++)
		{
			if (this->_trailing_visible_buttons.size() < 2)
			{
				if (this->_trailing_buttons[i]->get_leading_icon() != nullptr)
				{
					this->_trailing_visible_buttons.push_back(
						IconButton::init(this->_trailing_buttons[i]->get_leading_icon())
					);
				}
				else
				{
					this->_trailing_menu->add(this->_trailing_buttons[i]);
				}
			}
			else if (this->_trailing_visible_buttons.size() == 2)
			{
				if ((this->_trailing_buttons[i]->get_leading_icon() != nullptr) && this->_trailing_buttons.size() == 3)
				{
					this->_trailing_visible_buttons.push_back(
						IconButton::init(this->_trailing_buttons[i]->get_leading_icon())
					);
				}
				else
				{
					this->_trailing_visible_buttons.push_back(
						IconButton::init(modui::icons::ic_more_vert)
					);

					this->_trailing_menu->add(this->_trailing_buttons[i]);
				}
			}
			else
			{
				this->_trailing_menu->add(this->_trailing_buttons[i]);
			}
		}
	}

	void TopAppBar::_update_text_size()
	{
		if (this->_text.empty() || this->_font_size == 0.0f)
		{
			this->_text_size.x = 0.0f;
			this->_text_size.y = 0.0f;

			return;
		}

		this->_text_size = ImGui::GetFont()->CalcTextSizeA(this->_font_size, FLT_MAX, 0.0f, this->_text.c_str(), nullptr);
	}
}