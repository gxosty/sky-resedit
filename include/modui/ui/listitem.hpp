#pragma once

#include "../core/common.hpp"
#include "basebutton.hpp"

#include <string>

namespace modui::ui
{
	class ListItem : public BaseButton
	{
	public:
		enum class Type {
			ONE_LINED = 0,
			TWO_LINED,
			THREE_LINED
		};

	public:
		ListItem(
			const std::string& text,
			const std::string& supporting_text,
			const std::string& trailing_text,
			ImageID leading_icon,
			Widget* trailing_widget,
			Type type
		);

		static ListItem* init(
			const std::string& text = "",
			const std::string& supporting_text = "",
			const std::string& trailing_text = "",
			ImageID leading_icon = nullptr,
			Widget* trailing_widget = nullptr,
			Type type = Type::TWO_LINED
		);

		ListItem* set_text(const std::string& text) override;
		const std::string& get_text() const;

		ListItem* set_supporting_text(const std::string& supporting_text) override;
		const std::string& get_supporting_text() const;

		ListItem* set_trailing_text(const std::string& set_trailing_text) override;
		const std::string& get_trailing_text() const;

		ListItem* set_leading_icon(ImageID image) override;
		ImageID get_leading_icon() const;

		ListItem* set_trailing_widget(Widget* trailing_widget) override;
		Widget* get_trailing_widget() const;

		ListItem* on_release(ButtonInputCallback callback) override;

		void pre_render() override;
		void render() override;

		float get_wrapped_size_x() override;
		float get_wrapped_size_y() override;

		float calculate_size_x(float bounding_box_size_x) override;
		float calculate_size_y(float bounding_box_size_y) override;

	private:
		std::string _text;
		std::string _supporting_text;
		std::string _trailing_text;
		ImageID _leading_icon;
		Widget* _trailing_widget;
		Type _type;
		bool _state;

		float _font_size;
		float _supporting_text_font_size;
		float _trailing_text_font_size;

		Vec2 _text_size;
		Vec2 _supporting_text_size;
		Vec2 _trailing_text_size;
		float _leading_icon_size;

		void _update_text_size();
		void _update_supporting_text_size();
		void _update_trailing_text_size();
		void _update_leading_icon_size();

		// returns position in local coordinates to the widget
		void _get_all_pos(
			Vec2& text_pos,
			Vec2& supporting_text_pos,
			Vec2& leading_icon_pos,
			Vec2& trailing_pos);

		float _press_factor;
	};
}