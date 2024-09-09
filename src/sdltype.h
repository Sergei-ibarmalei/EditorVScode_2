#pragma once
#include "../inc/SDL.h"

#include "init.h"

#include "flags.h"
#ifdef LOGTOSCREEN
#include <iostream>
#endif

constexpr int window_w{ 1500 };
constexpr int window_h{ 1200 };


namespace ed
{
	class SDL: public Init
	{
	private:
		SDL_Renderer* render{ nullptr };
		SDL_Window* window{ nullptr };
		SDL_Event e;
		bool init(const char* name, int w, int h);

	public:
		SDL(const char* name);
		~SDL();
		SDL(const SDL&) = delete;
		SDL& operator=(const SDL&) = delete;
		SDL_Renderer* Render() { return render; }
		SDL_Event& Event() { return e; }
	};
}