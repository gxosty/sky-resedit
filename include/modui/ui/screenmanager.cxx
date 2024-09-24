#include <modui/ui/screenmanager.hpp>
#include <modui/core/exceptions.hpp>

namespace modui::ui
{
	ScreenManager::ScreenManager(animation::ScreenAnimation* animation) : Widget(),
		_animation{animation},
		_current_screen{nullptr},
		_next_screen{nullptr} {}

	ScreenManager* ScreenManager::init(animation::ScreenAnimation* animation) { return new ScreenManager(animation); }

	ScreenManager* ScreenManager::set_screen(const std::string& screen_name)
	{
		Screen* screen = this->get_screen(screen_name);

		if (screen == nullptr) throw exceptions::ScreenNotFoundException(screen_name);

		if (this->_animation)
		{
			this->_next_screen = screen;

			this->_animation
				->from_screen(this->_current_screen)
				->to_screen(this->_next_screen)
				->set_duration(100)
				->on_finish(MODUI_ANIMATION_FINISH_CALLBACK(this) {
					if (this->_current_screen)
						this->_current_screen->alpha = 1.0f;

					if (this->_next_screen)
						this->_next_screen->alpha = 1.0f;

					this->_current_screen = this->_next_screen;
					this->_next_screen = nullptr;
				})->start();
		}
		else
		{
			this->_current_screen = screen;
		}

		return this;
	}

	Screen* ScreenManager::get_screen(const std::string &screen_name)
	{
		for (auto child : this->_children)
		{
			if (((Screen*)child)->get_name() == screen_name) return (Screen*)child;
		}

		return nullptr;
	}

	ScreenManager* ScreenManager::add(Widget* widget)
	{
		Screen* screen = dynamic_cast<Screen*>(widget);

		if (screen == nullptr) throw exceptions::AddWidgetException("Only `Screen` objects can be added to `ScreenManager`");

		auto ret = Widget::add(widget);

		if (this->_children.size() == 1) this->set_screen(((Screen*)this->_children[0])->get_name());

		return (ScreenManager*)ret;
	}

	void ScreenManager::render()
	{
		if (this->_current_screen)
			this->_current_screen->render();

		if (this->_next_screen)
			this->_next_screen->render();
	}

	float ScreenManager::calculate_pos_x(float bounding_box_pos_x)
	{
		auto ret = Widget::calculate_pos_x(bounding_box_pos_x);

		if (this->_current_screen)
			this->_current_screen->calculate_pos_x(ret);

		if (this->_next_screen)
			this->_next_screen->calculate_pos_x(ret);

		return ret;
	}

	float ScreenManager::calculate_pos_y(float bounding_box_pos_y)
	{
		auto ret = Widget::calculate_pos_y(bounding_box_pos_y);

		if (this->_current_screen)
			this->_current_screen->calculate_pos_y(ret);

		if (this->_next_screen)
			this->_next_screen->calculate_pos_y(ret);

		return ret;
	}

	float ScreenManager::calculate_size_x(float bounding_box_size_x)
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

		if (this->_current_screen) this->_current_screen->calculate_size_x(x);
		if (this->_next_screen) this->_next_screen->calculate_size_x(x);
		this->_calculated_size.x = x;

		return x;
	}

	float ScreenManager::calculate_size_y(float bounding_box_size_y)
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

		if (this->_current_screen) this->_current_screen->calculate_size_y(y);
		if (this->_next_screen) this->_next_screen->calculate_size_y(y);
		this->_calculated_size.y = y;

		return y;
	}
}