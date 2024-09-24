#pragma once

#include "../common.hpp"

namespace modui
{
	class Color
	{
	public:
		Color(Col32 color = 0) : _color{color} {}

		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		Color(int32_t r, int32_t g, int32_t b, int32_t a)
			: _color{(a << 24) | ((b & 0xFF) << 16) | ((g & 0xFF) << 8) | (r & 0xFF)} {}
		#pragma GCC diagnostic pop

		static inline Color from_argb(Col32 argb)
		{
			return Color((argb & 0xFF000000) | ((argb >> 16) & 0xFF) | (argb & 0xFF00) | ((argb & 0xFF) << 16));
		}

		static inline Color from_rgba(Col32 rgba)
		{
			return Color(((rgba & 0xFF) << 24) | (((rgba >> 8) & 0xFF) << 16) | (((rgba >> 16) & 0xFF) << 8) | (rgba >> 24));
		}

		inline Col32 get() const
		{
			return this->_color;
		}

		inline Col32 get_alpha_applied(float alpha) const
		{
			return (int(((this->_color >> 24) & 0xFF) * alpha) << 24) | (this->_color & 0xFFFFFF);
		}

		inline void set(Col32 color)
		{
			this->_color = color;
		}

		inline int32_t r()
		{
			return this->_color & 0xFF;
		}

		inline int32_t g()
		{
			return (this->_color >> 8) & 0xFF;
		}

		inline int32_t b()
		{
			return (this->_color >> 16) & 0xFF;
		}

		inline int32_t a()
		{
			return (this->_color >> 24) & 0xFF;
		}

		inline operator Col32() const
		{
			return this->_color;
		}

		inline Color& operator=(Col32 ccolor)
		{
			this->_color = ccolor;
			return *this;
		}

	private:
		// A'B'G'R
		Col32 _color;
	};
}
