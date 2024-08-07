#pragma once

#include "../core/common.hpp"
#include "widget.hpp"

namespace modui::ui
{
	class Divider : public Widget
	{
	public:
		enum class Type {
			FULL = 0,
			INSENT
		};

	public:
		Divider(Type type);

		static Divider* init(Type type = Type::FULL);

		float get_wrapped_size_x() override;
		float get_wrapped_size_y() override;

		void render() override;

	private:
		Type _type;

	};
}