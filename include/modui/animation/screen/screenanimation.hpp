#pragma once

#include "../animation.hpp"
#include "../../ui/screen.hpp"

namespace modui::animation
{
	class ScreenAnimation : public Animation
	{
	public:
		ScreenAnimation() : Animation() {}

		ScreenAnimation* from_screen(ui::Screen* screen);
		ScreenAnimation* to_screen(ui::Screen* screen);

	protected:
		ui::Screen* _from_screen;
		ui::Screen* _to_screen;
	};
}