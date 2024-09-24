#pragma once

#include "../common.hpp"
#include "color.hpp"

#include <string>

namespace modui
{
	class Theme
	{
	public:
		Theme(ThemeID id);

		ThemeID get_id() const;

		Col32 operator()(ThemeColor color); // multiplies color alpha by `Theme::global_alpha`
		Color& operator[](ThemeColor color);

	public:
		static float global_alpha;

	private:
		ThemeID _id;
		Color _colors[static_cast<unsigned>(ThemeColor::ThemeColorSize)];
	};
}