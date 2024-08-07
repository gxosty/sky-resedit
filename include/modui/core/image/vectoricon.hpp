#include "vectorimage.hpp"

namespace modui::image
{
	class VectorIcon : public VectorImage
	{
	public:
		VectorIcon(NSVGimage* image);
	};
}