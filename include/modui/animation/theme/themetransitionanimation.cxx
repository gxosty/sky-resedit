#include <modui/animation/theme/themetransitionanimation.hpp>
#include <cstdio>

namespace modui::animation
{
	inline int32_t _clrmix(int32_t v1, int32_t v2, float factor)
	{
		return (v1 + (v2 - v1) * factor);
	}

	ThemeTransitionAnimation::ThemeTransitionAnimation(Theme** target_theme)
		: _target_theme{target_theme}, _transition_theme(0)
	{
		this->on_finish(MODUI_ANIMATION_FINISH_CALLBACK(this) {
			*this->_target_theme = this->_to_theme;
		});
	}

	ThemeTransitionAnimation* ThemeTransitionAnimation::from_theme(Theme* theme)
	{
		this->_from_theme = theme;

		return this;
	}

	ThemeTransitionAnimation* ThemeTransitionAnimation::to_theme(Theme* theme)
	{
		this->_to_theme = theme;

		return this;
	}

	void ThemeTransitionAnimation::update()
	{
		uint64_t current_time = uint64_t(ImGui::GetTime() * 1000);
		uint64_t start_time = this->_end_time - this->_duration;

		float factor = float(utils::map(current_time, start_time, this->_end_time, 0, 1000)) / 1000.0f;

		*this->_target_theme = &this->_transition_theme;

		for (unsigned i = 0; i < static_cast<unsigned>(ThemeColor::ThemeColorSize); i++)
		{
			this->_transition_theme[static_cast<ThemeColor>(i)] = Color(
				_clrmix(
					(*this->_from_theme)[static_cast<ThemeColor>(i)].r(),
					(*this->_to_theme)[static_cast<ThemeColor>(i)].r(),
					factor
				),
				_clrmix(
					(*this->_from_theme)[static_cast<ThemeColor>(i)].g(),
					(*this->_to_theme)[static_cast<ThemeColor>(i)].g(),
					factor
				),
				_clrmix(
					(*this->_from_theme)[static_cast<ThemeColor>(i)].b(),
					(*this->_to_theme)[static_cast<ThemeColor>(i)].b(),
					factor
				),
				_clrmix(
					(*this->_from_theme)[static_cast<ThemeColor>(i)].a(),
					(*this->_to_theme)[static_cast<ThemeColor>(i)].a(),
					factor
				)
			);

		}
	}
}