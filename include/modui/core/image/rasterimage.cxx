#include <modui/core/image/rasterimage.hpp>

#include <imgui/imgui.h>

#ifdef _WIN32
	#include <GL/gl.h>
#else // assume Android
	#include <GLES3/gl3.h>
#endif // _WIN32

namespace modui::image
{
	RasterImage::RasterImage(ImTextureID texture, unsigned width, unsigned height)
		: _texture{texture}, _width{width}, _height{height}
	{
		this->_image_type = ImageType::RASTER_IMAGE;
	}

	RasterImage::~RasterImage()
	{
		if (this->_texture != 0)
		{
			glDeleteTextures(1, (const GLuint*)&this->_texture);
			this->_texture = 0;
		}
	}

	void RasterImage::draw(Vec2 pos, Vec2 size, Col32 color)
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddImage(this->_texture, pos, pos + size, Vec2(0.0f, 0.0f), Vec2(1.0f, 1.0f), color);
	}
}