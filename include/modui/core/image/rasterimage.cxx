#include <modui/core/image/rasterimage.hpp>

#include <imgui/imgui.h>

namespace modui::image
{
	RasterImage::RasterImage(ImTextureID texture, unsigned width, unsigned height)
		: _texture{texture}, _width{width}, _height{height}
	{
		this->_image_type = ImageType::RASTER_IMAGE;
	}

	void RasterImage::draw(Vec2 pos, Vec2 size, Col32 color)
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddImage(this->_texture, pos, pos + size, Vec2(0.0f, 0.0f), Vec2(1.0f, 1.0f), color);
	}
}