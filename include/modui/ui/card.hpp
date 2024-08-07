#pragma once

#include "../core/common.hpp"
#include "linearlayout.hpp"

namespace modui::ui
{
	class FilledCard : public LinearLayout
	{
	public:
		FilledCard(LinearLayout::Orientation orientation);

		static FilledCard* init(
			LinearLayout::Orientation orientation = LinearLayout::Orientation::VERTICAL
		);

		void render() override;

	private:
		float _rounding;
	};
}