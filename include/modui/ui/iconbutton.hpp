#pragma once

#include "../core/common.hpp"
#include "../core/image/image.hpp"
#include "basebutton.hpp"

namespace modui::ui
{
	class IconButton : public BaseButton
	{
	public:
		IconButton(ImageID icon);
		static IconButton* init(ImageID icon = nullptr);

		IconButton* set_icon(ImageID icon) override;
		IconButton* set_icon_size(float icon_size) override;
		IconButton* set_state(bool state) override;
		IconButton* set_toggleable(bool toggleable) override;

		const ImageID get_icon();
		bool get_state() override;

		IconButton* on_release(ButtonInputCallback callback) override;

		void render() override;

		float get_wrapped_size_x() override;
		float get_wrapped_size_y() override;

		float calculate_size_x(float bounding_box_size_x) override;
		float calculate_size_y(float bounding_box_size_y) override;

	private:
		void _update_icon();

	protected:
		ImageID _icon;
		float _icon_size;
		float _rounding;
		bool _state;
		bool _toggleable;

		float _press_factor;
	};

	class FilledIconButton : public IconButton
	{
	public:
		FilledIconButton(ImageID icon);
		static FilledIconButton* init(ImageID icon = nullptr);

		void render() override;

	private:
	};

	class FilledTonalIconButton : public IconButton
	{
	public:
		FilledTonalIconButton(ImageID icon);
		static FilledTonalIconButton* init(ImageID icon = nullptr);

		void render() override;
	};

	class OutlinedIconButton : public IconButton
	{
	public:
		OutlinedIconButton(ImageID icon);
		static OutlinedIconButton* init(ImageID icon = nullptr);

		void render() override;
	};
}