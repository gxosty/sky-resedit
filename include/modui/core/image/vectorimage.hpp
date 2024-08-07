#pragma once

#include "image.hpp"
#include <imgui/imgui_impl_opengl3.h>
#include "../../thirdparty/nanosvg/nanosvg.h"

namespace modui::image
{
	class VectorImage : public Image
	{
	public:
		VectorImage(NSVGimage* image);

		void draw(Vec2 pos, Vec2 size, Col32 color) override;

	private:
		void _rasterize_image(const Vec2& size);

		NSVGimage* _image;
		ImTextureID _texture;
		float _last_min_size;
	};
}