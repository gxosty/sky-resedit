#pragma once

#include "../core/common.hpp"
#include "basebutton.hpp"

namespace modui::ui
{
	class BaseSlider : public BaseButton
	{
	public:
		BaseSlider(float min_value, float max_value, float reserved_width = 0.0f);

		Widget* set_value(float value) override;
		float get_value();

		Widget* on_slide(ButtonInputCallback callback) override;

		void render() override;

	private:
		ButtonInputCallback _on_slide_callback;

	protected:
		bool _is_sliding;
		float _min_value;
		float _max_value;
		float _value;
		float _reserved_width;

		virtual void on_slide_call();
	};
}