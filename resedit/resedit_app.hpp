#pragma once

#include <modui/modui.hpp>

namespace resedit
{
	class ResEditApp : public modui::App
	{
	public:
		ResEditApp();

		modui::ui::Widget* build() override;

		static void initialize();
		static ResEditApp* get_instance();

	private:
		static ResEditApp* instance;

		modui::ui::ScreenManager* _screen_manager;
	};
}