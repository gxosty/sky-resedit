#pragma once

#include "../core/common.hpp"

#include <functional>
#include <cstdint>

namespace modui::animation
{
	class Animation
	{
	public:
		friend class modui::App;

		Animation();

		Animation* set_duration(uint64_t milliseconds);

		bool start();
		virtual void update() = 0;
		Animation* on_finish(AnimationFinishCallback callback);

		bool is_running();

	protected:
		uint64_t _duration;
		uint64_t _end_time;

	private:
		AnimationFinishCallback _callback;
		bool _callback_ran;

		bool _update();
	};
}