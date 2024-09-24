#include "animation.hpp"
#include <modui/app.hpp>

#include <imgui/imgui.h>

namespace modui::animation
{
	Animation::Animation()
		: _duration{1000}, _end_time{0}, _callback_ran{false} {}

	Animation* Animation::set_duration(uint64_t milliseconds)
	{
		this->_duration = milliseconds;

		return this;
	}

	bool Animation::start()
	{
		this->_end_time = uint64_t(ImGui::GetTime() * 1000) + this->_duration;
		this->_callback_ran = false;

		if (auto app = modui::get_current_app())
		{
			app->add_running_animation(this);
			return true;
		}

		return false;
	}

	Animation* Animation::on_finish(AnimationFinishCallback callback)
	{
		this->_callback = std::move(callback);

		return this;
	}

	bool Animation::is_running()
	{
		return uint64_t(ImGui::GetTime() * 1000) < this->_end_time;
	}

	bool Animation::_update()
	{
		if (!this->is_running())
		{
			if (!this->_callback_ran)
			{
				this->_callback();
				this->_callback_ran = true;
			}			

			return false;
		}

		this->update();

		return true;
	}
}