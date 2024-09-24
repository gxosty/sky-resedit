#include <modui/ui/basebutton.hpp>
#include <modui/app.hpp>

namespace modui::ui
{
	BaseButton::BaseButton() : Widget(),
		_is_pressed{false},
		_is_released{false}
	{
		this->_clickable = true;
	}

	Widget* BaseButton::on_press(ButtonInputCallback callback)
	{
		this->_on_press_callback = std::move(callback);

		return this;
	}

	Widget* BaseButton::on_hold(ButtonInputCallback callback)
	{
		this->_on_hold_callback = std::move(callback);

		return this;
	}

	Widget* BaseButton::on_release(ButtonInputCallback callback)
	{
		this->_on_release_callback = std::move(callback);

		return this;
	}

	void BaseButton::on_press_call()
	{
		if (this->_on_press_callback)
			modui::get_current_app()->add_callback_to_queue(this, &this->_on_press_callback);
	}

	void BaseButton::on_hold_call()
	{
		if (this->_on_hold_callback)
			modui::get_current_app()->add_callback_to_queue(this, &this->_on_hold_callback);
	}

	void BaseButton::on_release_call()
	{
		if (this->_on_release_callback)
			modui::get_current_app()->add_callback_to_queue(this, &this->_on_release_callback);
	}

	void BaseButton::render()
	{
		ImGui::SetCursorScreenPos(this->_pos);

		if ((!ImGui::IsMouseDragging(0) || (ImGui::GetScrollMaxY() <= 0.0f)) && this->_clickable)
		{
			ImGui::PushID(this->_id);
			this->_is_released = ImGui::InvisibleButton(DEFAULT_ID, this->_calculated_size);
			ImGui::PopID();
		}
		else
		{
			ImGui::Dummy(this->_calculated_size);
		}


		this->_is_held = ImGui::IsItemActive();
		this->_is_pressed = ImGui::IsItemActivated();

		if (this->_is_pressed) this->on_press_call();
		if (this->_is_held) this->on_hold_call();
		if (this->_is_released) this->on_release_call();
	}
}