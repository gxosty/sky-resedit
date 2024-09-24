#pragma once

#include "core/basewidget.hpp"
#include "core/style/theme.hpp"
#include "core/style/thememanager.hpp"
#include "ui/widget.hpp"
#include "ui/iconbutton.hpp"
#include "animation/animation.hpp"
#include "animation/theme/themetransitionanimation.hpp"
#include <imgui/imgui.h>

#include <string>
#include <vector>
#include <queue>
#include <mutex>

namespace modui
{
	class App : core::BaseWidget
	{
		friend class ui::Widget;

	public:
		App();
		App(const std::string& window_title);

		// void run();
		virtual ui::Widget* build() = 0;

		App* set_window_title(const std::string& window_title);
		App* set_window_size(Vec2 window_size);
		App* set_window_fullscreen(bool fullscreen);
		App* set_window_open(bool open);
		App* set_window_closable(bool closable);
		App* set_window_collapsable(bool collapsable);
		App* set_window_alpha(float window_alpha);
		App* set_window_background_alpha(float background_window_alpha);

		Vec2 get_window_pos();
		Vec2 get_window_size();
		ThemeManager& get_theme_manager();

		Theme& get_current_theme();
		void set_current_theme(ThemeID theme_id);

		ImDrawListSplitter& get_draw_list_splitter();

		bool is_window_open();
		bool is_rendering();

		void pre_render();
		void render();
		void post_render();

		void add_control_widget(ui::IconButton* widget);
		void remove_control_widget(ui::IconButton* widget);

		void add_callback_to_queue(ui::Widget* widget, ButtonInputCallback* callback);
		void add_running_animation(animation::Animation* animation);
		void add_foreground_widget(ui::Widget* widget);
		void remove_foreground_widget(ui::Widget* widget);
		void run_in_ui_thread(void(*func)());

	protected:
		ImGuiWindow* _window;
		ui::Widget* _root_widget;
		bool _fullscreen;
		std::string _window_title;
		Vec2 _window_pos;
		Vec2 _window_size; // only when not fullscreen
		bool _window_open;
		bool _window_closable;
		bool _window_collapsable;

		ThemeManager _theme_manager;
		Theme* _current_theme;
		float _window_alpha;
		float _window_background_alpha;

		bool _prerendered;
		bool _rendering;

		std::vector<
			std::pair<ui::Widget*, ButtonInputCallback*>
		> _queued_callbacks;

		std::mutex _queued_ui_functions_mutex;
		std::queue<void*> _queued_ui_functions;

		std::vector<ui::Widget*> _control_widgets;
		std::vector<animation::Animation*> _running_animations;
		std::vector<ui::Widget*> _foreground_widgets;

		ImDrawListSplitter _draw_list_splitter;

		ui::Widget* _window_close_button;
		ui::Widget* _window_collapse_button;
		animation::ThemeTransitionAnimation* _theme_transition_animation;

		void _render_window_title();
		void _update_animations();
		void _render_foreground_widgets();

		void _drain_queued_callbacks();
		void _drain_queued_ui_functions();
	};
}