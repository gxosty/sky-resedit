#include <modui/ui/card.hpp>
#include <modui/app.hpp>

namespace modui::ui
{
	FilledCard::FilledCard(LinearLayout::Orientation orientation) : LinearLayout(orientation),
		_rounding{utils::dp(16)} {}

	FilledCard* FilledCard::init(LinearLayout::Orientation orientation) { return new FilledCard(orientation); }

	void FilledCard::render()
	{
		ImDrawList* draw_list =  ImGui::GetWindowDrawList();
		Theme& theme = this->get_theme();
		ImDrawListSplitter& draw_list_splitter = modui::get_current_app()->get_draw_list_splitter();

		draw_list_splitter.Split(draw_list, 2);
		draw_list_splitter.SetCurrentChannel(draw_list, 1);
		Widget::push_on_card();
		LinearLayout::render();
		Widget::pop_on_card();

		draw_list_splitter.SetCurrentChannel(draw_list, 0);

		draw_list->AddRectFilled(
			this->_pos,
			this->_pos + this->_calculated_size,
			theme(ThemeColor::SurfaceContainerHighest),
			this->_rounding
		);

		draw_list_splitter.Merge(draw_list);
	}
}