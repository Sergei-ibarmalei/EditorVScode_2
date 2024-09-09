#include "sdltype.h"


namespace ed
{
	bool SDL::init(const char* name, int w, int h)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) return false;
		window = SDL_CreateWindow(name,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			w,
			h,
			SDL_WINDOW_SHOWN);
		if (!window)
		{
#ifdef LOGTOSCREEN
			std::cout << "Cannot initialize window, abort.\n";
#endif
			return false;
		}
		render = SDL_CreateRenderer(window,
			-1,
			SDL_RENDERER_ACCELERATED |
			SDL_RENDERER_PRESENTVSYNC);
		if (!render)
		{
#ifdef LOGTOSCREEN
			std::cout << "Cannot initialize render, abort.\n";
#endif
			return false;
		}
		/*int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
#ifdef LOGTOSCREEN
			std::cout << "Cannot initialize SDL_image, abort.\n";
#endif
			return false;
		}*/
		return true;
	}



	SDL::SDL(const char* name)
	{
		if (!name)
		{
			Negate();
#ifdef LOGTOSCREEN
			std::cout << "Cannot initiate SDL, app name is absent.\n";
#endif
			return;
		}
		if (!init(name, window_w, window_h))
		{
			Negate();
		}

	}


	SDL::~SDL()
	{
		if (render)
		{
			SDL_DestroyRenderer(render);
			render = nullptr;
		}
		if (window)
		{
			SDL_DestroyWindow(window);
			window = nullptr;
		}
		//IMG_Quit();
		SDL_Quit();
	}
}