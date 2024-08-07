#include <modui/ui/screen.hpp>
#include <modui/core/exceptions.hpp>

namespace modui::ui
{
	Screen::Screen(const std::string& name) : Widget(),
		_name{name} {}

	Screen* Screen::init(const std::string& name) { return new Screen(name); }

	Screen* Screen::set_name(const std::string& name)
	{
		this->_name = name;

		return this;
	}

	const std::string& Screen::get_name()
	{
		return this->_name;
	}

	Screen* Screen::add(Widget* widget)
	{
		if (!this->_children.empty())
			throw exceptions::AddWidgetException("Only one widget can be added to `modui::ui::Screen` object");

		return (Screen*)Widget::add(widget);
	}

	void Screen::render()
	{
		if (!this->_children.empty())
			this->_children[0]->render();
	}

	// float Screen::get_wrapped_size_x()
	// {
	// 	return utils::dp(240);
	// }

	// float Screen::get_wrapped_size_y()
	// {
	// 	return ;
	// }

	float Screen::calculate_pos_x(float bounding_box_pos_x)
	{
		auto ret = Widget::calculate_pos_x(bounding_box_pos_x);

		if (!this->_children.empty())
			this->_children[0]->calculate_pos_x(ret);

		return ret;
	}

	float Screen::calculate_pos_y(float bounding_box_pos_y)
	{
		auto ret = Widget::calculate_pos_y(bounding_box_pos_y);

		if (!this->_children.empty())
			this->_children[0]->calculate_pos_y(ret);

		return ret;
	}

	float Screen::calculate_size_x(float bounding_box_size_x)
	{
		float x = this->_size.x;
		this->_bounding_box_size.x = bounding_box_size_x;

		if ((x == MODUI_SIZE_WIDTH_FULL) || (x == MODUI_SIZE_WIDTH_WRAP))
		{
			x = bounding_box_size_x;
		}
		else if (x < 0.0f)
		{
			x = bounding_box_size_x + x;
		}

		if (!this->_children.empty()) this->_children[0]->calculate_size_x(x);
		this->_calculated_size.x = x;

		return x;
	}

	float Screen::calculate_size_y(float bounding_box_size_y)
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

		if (!this->_children.empty()) this->_children[0]->calculate_size_y(y);
		this->_calculated_size.y = y;

		return y;
	}
}