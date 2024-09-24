#pragma once

#include "../../core/common.hpp"
#include "../../core/style/theme.hpp"
#include "../animation.hpp"

namespace modui::animation
{
	class ThemeTransitionAnimation : public Animation
	{
	public:
		ThemeTransitionAnimation(Theme** target_theme);

		ThemeTransitionAnimation* from_theme(Theme* theme);
		ThemeTransitionAnimation* to_theme(Theme* theme);

		void update() override;

	private:
		Theme _transition_theme;
		Theme* _from_theme;
		Theme* _to_theme;
		Theme** _target_theme;
	};
}