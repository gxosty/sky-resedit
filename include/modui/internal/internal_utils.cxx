#include "internal_utils.hpp"

namespace modui
{
	// extern float __screen_density;

	namespace internal
	{
		int __next_id_for_widget = 10000;

		int __get_next_id_for_widget()
		{
			return __next_id_for_widget++;
		}
	}
}