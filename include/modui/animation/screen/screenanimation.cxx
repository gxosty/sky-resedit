#include "screenanimation.hpp"

namespace modui::animation
{
	ScreenAnimation* ScreenAnimation::from_screen(ui::Screen* screen)
	{
		this->_from_screen = screen;

		return this;
	}

	ScreenAnimation* ScreenAnimation::to_screen(ui::Screen* screen)
	{
		this->_to_screen = screen;
		
		return this;
	}
}