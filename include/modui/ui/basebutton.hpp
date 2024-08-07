#pragma once

#include "../core/common.hpp"
#include "widget.hpp"

namespace modui::ui
{
	class BaseButton : public Widget
	{
	public:
		BaseButton();

		Widget* on_press(ButtonInputCallback callback) override;
		Widget* on_hold(ButtonInputCallback callback) override;
		Widget* on_release(ButtonInputCallback callback) override;

		void render() override;

	private:
		ButtonInputCallback _on_press_callback;
		ButtonInputCallback _on_hold_callback;
		ButtonInputCallback _on_release_callback;

	protected:
		bool _is_pressed;
		bool _is_held;
		bool _is_released;

		virtual void on_press_call();
		virtual void on_hold_call();
		virtual void on_release_call();
	};
}