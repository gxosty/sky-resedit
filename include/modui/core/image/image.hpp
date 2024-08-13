#pragma once

#include "../common.hpp"

#include <string>

namespace modui::image
{
	enum class ImageType
	{
		RASTER_IMAGE = 0,
		VECTOR_IMAGE,
		SELECTABLE,
		ICON,
		AVATAR // TODO
	};

	class Image
	{
	public:
		virtual ~Image() = default;

		static ImageID load(const std::string& file_path, ImageType image_type = ImageType::RASTER_IMAGE);
		static ImageID load_from_memory(void* buffer, unsigned buffer_size = -1, ImageType image_type = ImageType::RASTER_IMAGE);

		static void unload(ImageID image);

		static ImageID make_selectable(ImageID unselected_image, ImageID selected_image);

		virtual void draw(Vec2 pos, Vec2 size, Col32 color) = 0;

		ImageType get_image_type() const;

	protected:
		ImageType _image_type;
	};
}