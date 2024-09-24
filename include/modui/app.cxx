#include <modui/app.hpp>
#include <modui/core/image/image.hpp>
#include <modui/ui/iconbutton.hpp>
#include "internal/internal_core.hpp"
#include "internal/internal_utils.hpp"

#include <cmath>
#include <stdio.h>

#include <imgui/imgui.h>

namespace modui
{
	namespace icons
	{
		char ic_close_bfr[] = R"=(<svg xmlns="http://www.w3.org/2000/svg" height="24px" viewBox="0 -960 960 960" width="24px" fill="#e8eaed"><path d="m256-200-56-56 224-224-224-224 56-56 224 224 224-224 56 56-224 224 224 224-56 56-224-224-224 224Z"/></svg>)=""\x00";
		ImageID ic_close = modui::image::Image::load_from_memory(ic_close_bfr, -1, modui::image::ImageType::ICON);

		char ic_collapse_content_bfr[] = R"=(<svg xmlns="http://www.w3.org/2000/svg" height="24px" viewBox="0 -960 960 960" width="24px" fill="#e8eaed"><path d="M440-440v240h-80v-160H200v-80h240Zm160-320v160h160v80H520v-240h80Z"/></svg>)=""\x00";
		ImageID ic_collapse_content = modui::image::Image::load_from_memory(ic_collapse_content_bfr, -1, modui::image::ImageType::ICON);

		char ic_expand_content_bfr[] = R"=(<svg xmlns="http://www.w3.org/2000/svg" height="24px" viewBox="0 -960 960 960" width="24px" fill="#e8eaed"><path d="M200-200v-240h80v160h160v80H200Zm480-320v-160H520v-80h240v240h-80Z"/></svg>)=""\x00";
		ImageID ic_expand_content = modui::image::Image::load_from_memory(ic_expand_content_bfr, -1, modui::image::ImageType::ICON);

		static ImageID selectable_collapse = modui::image::Image::make_selectable(
			ic_collapse_content,
			ic_expand_content
		);
	}

	App::App() :
		_fullscreen{false},
		_root_widget{nullptr},
		_window_title{"Window " + std::to_string(modui::internal::__get_next_id_for_widget())},
		_window_pos{utils::dp(50), utils::dp(50)},
		_window_size{utils::dp(150), utils::dp(150)},
		_prerendered{false},
		_rendering{false},
		_window_open{true},
		_window_closable{true},
		_window_collapsable{false},
		_window_alpha{1.0f},
		_window_background_alpha{1.0f},
		_theme_transition_animation{nullptr}
	{
		this->_theme_manager.add(Theme(DEFAULT_THEME_LIGHT));
		this->_theme_manager.add(Theme(DEFAULT_THEME_DARK));

		Theme& dark_theme = this->_theme_manager.get(DEFAULT_THEME_DARK);

		dark_theme[ThemeColor::Primary]                 = MODUI_COLOR_HEX(0xFFD0BCFF);
		dark_theme[ThemeColor::OnPrimary]               = MODUI_COLOR_HEX(0xFF381E72);
		dark_theme[ThemeColor::PrimaryContainer]        = MODUI_COLOR_HEX(0xFF4F378B);
		dark_theme[ThemeColor::OnPrimaryContainer]      = MODUI_COLOR_HEX(0xFFEADDFF);

		dark_theme[ThemeColor::Secondary]               = MODUI_COLOR_HEX(0xFFCCC2DC);
		dark_theme[ThemeColor::OnSecondary]             = MODUI_COLOR_HEX(0xFF332D41);
		dark_theme[ThemeColor::SecondaryContainer]      = MODUI_COLOR_HEX(0xFF4A4458);
		dark_theme[ThemeColor::OnSecondaryContainer]    = MODUI_COLOR_HEX(0xFFE8DEF8);

		dark_theme[ThemeColor::Tertiary]                = MODUI_COLOR_HEX(0xFFEFB8C8);
		dark_theme[ThemeColor::OnTertiary]              = MODUI_COLOR_HEX(0xFF492532);
		dark_theme[ThemeColor::TertiaryContainer]       = MODUI_COLOR_HEX(0xFF633B48);
		dark_theme[ThemeColor::OnTertiaryContainer]     = MODUI_COLOR_HEX(0xFFFFD8E4);

		dark_theme[ThemeColor::Error]                   = MODUI_COLOR_HEX(0xFFF2B8B5);
		dark_theme[ThemeColor::OnError]                 = MODUI_COLOR_HEX(0xFF601410);
		dark_theme[ThemeColor::ErrorContainer]          = MODUI_COLOR_HEX(0xFF8C1D18);
		dark_theme[ThemeColor::OnErrorContainer]        = MODUI_COLOR_HEX(0xFFF9DEDC);

		dark_theme[ThemeColor::Surface]                 = MODUI_COLOR_HEX(0xFF141218);
		dark_theme[ThemeColor::OnSurface]               = MODUI_COLOR_HEX(0xFFE6E0E9);
		dark_theme[ThemeColor::SurfaceVariant]          = MODUI_COLOR_HEX(0xFF49454F);
		dark_theme[ThemeColor::OnSurfaceVariant]        = MODUI_COLOR_HEX(0xFFCAC4D0);
		dark_theme[ThemeColor::SurfaceContainerHighest] = MODUI_COLOR_HEX(0xFF36343B);
		dark_theme[ThemeColor::SurfaceContainerHigh]    = MODUI_COLOR_HEX(0xFF2B2930);
		dark_theme[ThemeColor::SurfaceContainer]        = MODUI_COLOR_HEX(0xFF211F26);
		dark_theme[ThemeColor::SurfaceContainerLow]     = MODUI_COLOR_HEX(0xFF1D1B20);
		dark_theme[ThemeColor::SurfaceContainerLowest]  = MODUI_COLOR_HEX(0xFF0F0D13);
		dark_theme[ThemeColor::InverseSurface]          = MODUI_COLOR_HEX(0xFFE6E0E9);
		dark_theme[ThemeColor::InverseOnSurface]        = MODUI_COLOR_HEX(0xFF322F35);
		dark_theme[ThemeColor::SurfaceTint]             = MODUI_COLOR_HEX(0xFFD0BCFF);

		dark_theme[ThemeColor::Outline]                 = MODUI_COLOR_HEX(0xFF938F99);
		dark_theme[ThemeColor::OutlineVariant]          = MODUI_COLOR_HEX(0xFF49454F);

		this->set_current_theme(DEFAULT_THEME_LIGHT);
		this->_queued_callbacks.reserve(10);
		this->_running_animations.reserve(5);
		this->_control_widgets.resize(2); // window close button and collapse button

		this->_theme_transition_animation = new animation::ThemeTransitionAnimation(&this->_current_theme);
	};

	App::App(const std::string& window_title) : App()
	{
		this->set_window_title(window_title);
	}

	App* App::set_window_title(const std::string& window_title)
	{
		this->_window_title = window_title;
		return this;
	}

	App* App::set_window_size(Vec2 window_size)
	{
		this->_window_size = window_size;
		return this;
	}

	App* App::set_window_fullscreen(bool fullscreen)
	{
		this->_fullscreen = fullscreen;
		return this;
	}

	App* App::set_window_open(bool open)
	{
		this->_window_open = open;

		return this;
	}

	App* App::set_window_closable(bool closable)
	{
		this->_window_closable = closable;

		return this;
	}

	App* App::set_window_collapsable(bool collapsable)
	{
		this->_window_collapsable = collapsable;

		return this;
	}

	App* App::set_window_alpha(float alpha)
	{
		this->_window_alpha = alpha;

		return this;
	}

	App* App::set_window_background_alpha(float alpha)
	{
		this->_window_background_alpha = alpha;

		return this;
	}

	void App::pre_render()
	{
		if (this->_prerendered) return;

		modui::internal::__set_current_app(this);
		this->_root_widget = this->build()->build_widget();
		this->_root_widget->pre_render();
		const float close_button_size = utils::dp(30);
		this->_window_close_button = ui::IconButton::init(modui::icons::ic_close)
			->set_size(Vec2(close_button_size, close_button_size))
			->on_release(MODUI_CALLBACK(this) {
				this->_window_open = false;
			});
		this->_window_close_button->pre_render();
		this->_window_collapse_button = ui::IconButton::init(modui::icons::selectable_collapse)
			->set_size(ImVec2(close_button_size, close_button_size))
			->set_state(false)
			->set_toggleable(true)
			->on_release(MODUI_CALLBACK(this) {
				bool state = this_widget->get_state();

				const int buttons = \
					(this->_window_closable ? 1 : 0) +
					(this->_window_collapsable ? 1 : 0) +
					(this->_control_widgets.size() > 2 ? this->_control_widgets.size() - 2 : 0);

				const float width = ImGui::CalcTextSize(this->_window_title.c_str()).x + utils::dp(10) * (buttons > 0 ? 3.0f : 2.0f) + utils::dp(30) * buttons + utils::dp(5) * fmax(buttons - 1, 0);

				if (state)
				{
					this->_window->Pos.x = this->_window_pos.x + this->_window_size.x - width;
				}
				else
				{
					this->_window->Pos.x -= this->_window_size.x - width;
				}
			});
		this->_window_collapse_button->pre_render();
		modui::internal::__set_current_app(nullptr);

		this->_control_widgets[0] = this->_window_close_button;
		this->_control_widgets[1] = this->_window_collapse_button;

		bool opened = false;

		if (!this->_fullscreen)
			ImGui::SetNextWindowSize(this->_window_size);
		
		ImGui::Begin(this->_window_title.c_str(), &opened);
		this->_window = ImGui::GetCurrentWindow();
		ImGui::End();

		this->_prerendered = true;
	}

	void App::render()
	{
		this->_rendering = true;
		modui::internal::__set_current_app(this);

		this->_update_animations();
		this->_drain_queued_ui_functions();

		if (!this->_window_open)
		{
			this->_drain_queued_callbacks();
			modui::internal::__set_current_app(nullptr);
			this->_rendering = false;
			return;
		}

		int window_flags =
			  ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoTitleBar;

		int pop_style_var_count = 10;
		int pop_style_col_count = 5;

		bool collapsed = this->_window_collapse_button->get_state();

		if (this->_fullscreen)
		{
			window_flags |=
				  ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoMove;

			this->_window_pos = ImVec2(0.0f, 0.0f);
			ImGui::SetNextWindowPos(this->_window_pos);
			ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		}
		else if (collapsed)
		{
			window_flags |= ImGuiWindowFlags_NoResize;

			const float title_height = fmax(ImGui::GetFontSize() * 2.0f, utils::dp(30));
			const int buttons = \
				(this->_window_closable ? 1 : 0) +
				(this->_window_collapsable ? 1 : 0) +
				(this->_control_widgets.size() > 2 ? this->_control_widgets.size() - 2 : 0);

			ImGui::SetNextWindowSize(ImVec2(
				ImGui::CalcTextSize(this->_window_title.c_str()).x + utils::dp(10) * (buttons > 0 ? 3.0f : 2.0f) + utils::dp(30) * buttons + utils::dp(5) * fmax(buttons - 1, 0),
				title_height
			));
		}
		else
		{
			ImGui::SetNextWindowSize(this->_window_size);
		}

		Theme& theme = this->get_current_theme();
		float bkp_theme_alpha = Theme::global_alpha;
		Theme::global_alpha = this->_window_alpha;

		ImGui::PushStyleColor(ImGuiCol_WindowBg, theme(ThemeColor::Surface));
		ImGui::PushStyleColor(ImGuiCol_ChildBg, theme(ThemeColor::Surface));
		ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);
		ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, 0);
		ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, theme(ThemeColor::Secondary));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, this->_fullscreen ? 0.0f : utils::dp(10));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0.0f, 0.0f));

		ImGui::SetNextWindowBgAlpha(this->_window_background_alpha);
		if (!ImGui::Begin(this->_window_title.c_str(), &this->_window_open, window_flags))
		{
			ImGui::End();

			Theme::global_alpha = bkp_theme_alpha;
			ImGui::PopStyleVar(pop_style_var_count);
			ImGui::PopStyleColor(pop_style_col_count);
			return;
		}

		if (!ImGui::IsWindowCollapsed())
		{
			if (!this->_fullscreen) _render_window_title();

			if (!collapsed)
			{	
				Vec2 cursor_pos = ImGui::GetCursorScreenPos();
				Vec2 avail_size = ImGui::GetContentRegionAvail();

				this->_root_widget->calculate_pos_and_size(cursor_pos, avail_size);
				this->_root_widget->render();
			}
		}

		this->_render_foreground_widgets();		

		this->_window->ScrollMax.y = 0;
		ImGui::End();

		Theme::global_alpha = bkp_theme_alpha;
		ImGui::PopStyleVar(pop_style_var_count);
		ImGui::PopStyleColor(pop_style_col_count);

		if (!collapsed)
		{
			this->_window_size = this->_window->Size;
		}

		this->_window_pos = this->_window->Pos;

		this->_drain_queued_callbacks();
		modui::internal::__set_current_app(nullptr);
		this->_rendering = false;
	}

	void App::post_render()
	{
		this->_prerendered = false;
		delete this->_root_widget;
		this->_root_widget = nullptr;
	}

	void App::add_control_widget(ui::IconButton* widget)
	{
		const float close_button_size = utils::dp(30);
		widget->set_size(Vec2(close_button_size, close_button_size));
		this->_control_widgets.push_back(widget);
	}

	void App::remove_control_widget(ui::IconButton* widget)
	{
		for (int i = 0; i < this->_control_widgets.size(); i++)
		{
			if (this->_control_widgets[i] == widget)
			{
				this->_control_widgets.erase(this->_control_widgets.begin() + i);
				return;
			}
		}
	}

	void App::add_callback_to_queue(ui::Widget* widget, ButtonInputCallback* callback)
	{
		this->_queued_callbacks.emplace_back(widget, callback);
	}

	void App::add_running_animation(animation::Animation* animation)
	{
		this->_running_animations.push_back(animation);
	}

	void App::add_foreground_widget(ui::Widget* widget)
	{
		this->_foreground_widgets.push_back(widget);
	}

	void App::remove_foreground_widget(ui::Widget* widget)
	{
		for (int i = 0; i < this->_foreground_widgets.size(); i++)
		{
			if (this->_foreground_widgets[i] == widget)
			{
				this->_foreground_widgets.erase(this->_foreground_widgets.begin() + i);
				return;
			}
		}
	}

	void App::run_in_ui_thread(void(*func)())
	{
		std::lock_guard<std::mutex> lock(this->_queued_ui_functions_mutex);
		this->_queued_ui_functions.push(reinterpret_cast<void*>(func));
	}

	Vec2 App::get_window_pos()
	{
		return this->_window_pos;
	}

	Vec2 App::get_window_size()
	{
		return this->_window_size;
	}

	ThemeManager& App::get_theme_manager()
	{
		return this->_theme_manager;
	}

	Theme& App::get_current_theme()
	{
		return *this->_current_theme;
	}

	void App::set_current_theme(ThemeID theme_id)
	{
		if (this->_theme_transition_animation && this->_theme_transition_animation
				->from_theme(this->_current_theme)
				->to_theme(&this->_theme_manager.get(theme_id))
				->set_duration(250)
				->start()) {}
		else
		{
			this->_current_theme = &this->_theme_manager.get(theme_id);
		}
	}

	ImDrawListSplitter& App::get_draw_list_splitter()
	{
		return this->_draw_list_splitter;
	}

	bool App::is_window_open()
	{
		return this->_window_open;
	}

	bool App::is_rendering()
	{
		return this->_rendering;
	}

	void App::_render_window_title()
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		const Vec2 cursor_pos = ImGui::GetCursorScreenPos();

		const float font_size = ImGui::GetFontSize();
		const float padding = utils::dp(10);
		const float button_size = utils::dp(30);
		const float title_height = fmax(font_size * 2.0f, button_size);

		Vec2 title_pos = Vec2(cursor_pos.x + padding, cursor_pos.y + (title_height - font_size) / 2.0f);
		Vec2 button_pos = Vec2(cursor_pos.x + ImGui::GetContentRegionAvail().x - button_size - padding, cursor_pos.y + (title_height - button_size) / 2.0f);

		draw_list->AddText(ImGui::GetFont(), font_size, title_pos, this->get_current_theme()(ThemeColor::Primary), this->_window_title.c_str(), nullptr);

		if (this->_window_closable)
		{
			this->_window_close_button->calculate_pos_and_size(button_pos, Vec2(button_size, button_size));
			this->_window_close_button->render();

			button_pos.x -= utils::dp(5) + button_size;
		}

		if (this->_window_collapsable)
		{
			this->_window_collapse_button->calculate_pos_and_size(button_pos, Vec2(button_size, button_size));
			this->_window_collapse_button->render();

			button_pos.x -= utils::dp(5) + button_size;
		}

		for (int i = 2; i < this->_control_widgets.size(); i++)
		{
			this->_control_widgets[i]->calculate_pos_and_size(button_pos, Vec2(button_size, button_size));
			this->_control_widgets[i]->render();

			button_pos.x -= utils::dp(5) + button_size;
		}

		if (!this->_window_collapse_button->get_state())
			ImGui::SetCursorScreenPos(Vec2(cursor_pos.x, cursor_pos.y + title_height));
	}

	void App::_render_foreground_widgets()
	{
		for (auto widget : this->_foreground_widgets)
		{
			widget->calculate_pos_and_size(
				widget->get_pos(),
				widget->get_size()
			);
			widget->render();
		}
	}

	void App::_update_animations()
	{
		if (this->_running_animations.empty()) return;

		for (int i = this->_running_animations.size() - 1; i >= 0; i--)
		{
			if (!this->_running_animations[i]->_update())
			{
				this->_running_animations.erase(this->_running_animations.begin() + i);
			}
		}
	}

	void App::_drain_queued_callbacks()
	{
		for (auto& i : this->_queued_callbacks)
		{
			(*(i.second))(i.first);
		}

		this->_queued_callbacks.clear();
	}

	void App::_drain_queued_ui_functions()
	{
		if (this->_queued_ui_functions.empty()) return;

		std::lock_guard<std::mutex> lock(this->_queued_ui_functions_mutex);

		do {
			reinterpret_cast<void(*)()>(this->_queued_ui_functions.front())();
			this->_queued_ui_functions.pop();
		} while(!this->_queued_ui_functions.empty());
	}
}