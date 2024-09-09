#include "rect.h"


namespace ed
{
#ifdef LOGTOSCREEN
	std::ostream& operator<<(std::ostream& os, const Rect& r)
	{
		return os << "[" << r.x << " : " << r.y << "]\n";
	}
#endif
}