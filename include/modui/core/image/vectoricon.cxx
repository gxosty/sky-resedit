#include <modui/core/image/vectoricon.hpp>

namespace modui::image
{
	VectorIcon::VectorIcon(NSVGimage* image) : VectorImage(image)
	{
		this->_image_type = ImageType::ICON;
	}
}