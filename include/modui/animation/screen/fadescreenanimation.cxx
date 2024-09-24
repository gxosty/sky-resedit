#include "fadescreenanimation.hpp"

#include <cstdio>

namespace modui::animation
{
	void FadeScreenAnimation::update()
	{
		uint64_t current_time = uint64_t(ImGui::GetTime() * 1000);
		uint64_t start_time = this->_end_time - this->_duration;

		float factor = float(utils::map(current_time, start_time, this->_end_time, 0, 1000)) / 1000.0f;

		if (this->_from_screen)
		{
			this->_from_screen->alpha = 1.0f - factor;
		}

		if (this->_to_screen)
		{
			this->_to_screen->alpha = factor;
		}
	}
}