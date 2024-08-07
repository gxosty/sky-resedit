#include <modui/core/image/selectableimage.hpp>

#include <cstdio>

namespace modui::image
{
	SelectableImage::SelectableImage(ImageID unselected_image, ImageID selected_image) : Image(),
		_unselected_image{unselected_image},
		_selected_image{selected_image},
		_selected{false}
	{
		this->_image_type = ImageType::SELECTABLE;
	}

	void SelectableImage::set_selected(bool selected)
	{
		this->_selected = selected;
	}

	void SelectableImage::draw(Vec2 pos, Vec2 size, Col32 color)
	{
		if (this->_selected)
		{
			this->_selected_image->draw(pos, size, color);
		}
		else
		{
			this->_unselected_image->draw(pos, size, color);
		}
	}
}