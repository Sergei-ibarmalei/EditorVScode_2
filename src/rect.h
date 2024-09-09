#pragma once
#include "../inc/SDL.h"
//#include <SDL_image.h>
#include "flags.h"

#ifdef LOGTOSCREEN
#include <iostream>
#endif


namespace ed
{
	struct Rect : public SDL_Rect
	{
	public:
		Rect(int x = 0, int y = 0, int w = 0, int h = 0) : SDL_Rect{ x, y, w, h } {}
		bool operator=(const Rect& r) const
		{
			if ((y + h) <= r.y)		return false;
			if (y >= (r.y + r.h))	return false;
			if ((x + w) >= (r.x + r.w))	return false;
			if (x >= (r.x + r.w)) return false;
			return true;
		}

		bool operator==(const Rect* r) const
		{
			if ((y + h) <= r->y) return false;
			if (y >= (r->y + r->h)) return false;
			if ((x + w) >= (r->x + r->w)) return false;
			if (x >= (r->x + r->w)) return false;
			return true;
		}

		bool operator*(const Rect& r)
		{
			if ((y > r.y) && ((y + h) < r.y + r.h) && (x > r.x) && ((x + w) < r.x + r.w))
				return true;
			return false;
		}

		bool operator*(const Rect* r)
		{
			if ((y > r->y) && ((y + h) < r->y + r->h) && (x > r->x) && ((x + w) < r->x + r->w))
				return false;
			return true;
		}
#ifdef LOGTOSCREEN
		friend std::ostream& operator<<(std::ostream& os, const Rect& r);
#endif
	};
}