#pragma once

#include "image.hpp"

namespace modui::image
{
	class RasterImage : public Image
	{
	public:
		RasterImage(/* GLuint */ ImTextureID texture, unsigned width, unsigned height);

		void draw(Vec2 pos, Vec2 size, Col32 color) override;

	private:
		ImTextureID _texture;
		unsigned _width;
		unsigned _height;
	};
}