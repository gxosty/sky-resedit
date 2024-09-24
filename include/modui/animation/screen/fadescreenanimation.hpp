#pragma once

#include "screenanimation.hpp"

namespace modui::animation
{
	class FadeScreenAnimation : public ScreenAnimation
	{
	public:
		FadeScreenAnimation() : ScreenAnimation() {}

		static inline FadeScreenAnimation* init() { return new FadeScreenAnimation(); }

		void update() override;
	};
}