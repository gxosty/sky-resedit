#pragma once

#include "image.hpp"

namespace modui::image
{
	class SelectableImage : public Image
	{
	public:
		SelectableImage(ImageID unselected_image, ImageID selected_image);

		void set_selected(bool selected);

		void draw(Vec2 pos, Vec2 size, Col32 color) override;

	private:
		ImageID _unselected_image;
		ImageID _selected_image;
		bool _selected;
	};
}