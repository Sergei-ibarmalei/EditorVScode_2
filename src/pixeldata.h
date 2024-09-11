#pragma once
#include "../inc/SDL.h"

typedef struct
{
	SDL_Point corner;
	bool inMainRect;
	bool isFire;
	int pixelColor;

	// remember the position of pixel's color
	// in colored array (colors.h)
	int posInColoredArray;
}pixelData_t;

