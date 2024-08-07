#include "internal_core.hpp"

namespace modui
{
	extern App* __current_app;
}

namespace modui::internal
{
	void __set_current_app(App* app)
	{
		__current_app = app;
	}
}