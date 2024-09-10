#include "pixel.h"

#define GREY  0xc0u, 0xc0u, 0xc0u, 0xffu
#define WHITE 0xffu, 0xffu, 0xffu, 0xffu
#define RED 0xffu, 0u, 0u, 0xffu
constexpr int redflag = 0xff0000;
constexpr int greenflag = 0x00ff00;
constexpr int blueflag = 0x0000ff;

namespace ed
{
	Pixel::Pixel()
	{
		pixelRect = new Rect;
	}

	void Pixel::SetPixelData(const pixelData_t& data)
	{
		pixelRect->x = data.corner.x;
		pixelRect->y = data.corner.y;
		pixelRect->w = workPixelSide;
		pixelRect->h = workPixelSide;
		pixelColor = data.pixelColor;
		if (data.pixelColor > -1) filled = true;

		// is pixel in work table is choosen for mainrect
		// - has red border
		mainrectFilled = data.inMainRect;

		positionInColoredArray = data.posInColoredArray;

	}





	void Pixel::SetColor(const int color)
	{
		pixelColor = color;
	}

	void Pixel::RememberPositionInColoredTable(int positionInColoredTable)
	{
		positionInColoredArray = positionInColoredTable;
	}

	void Pixel::Draw(SDL_Renderer* const r, bool showingOneMainRect)
	{

		if (!filled)
		{
			// we colored mainrect pixels by red only if showingOneMainRect is false
			// if we have one big main rect in the screen, we do not 
			// show red border of main rect pixels
			if (mainrectFilled && !showingOneMainRect) SDL_SetRenderDrawColor(r, RED);
			else SDL_SetRenderDrawColor(r, GREY);
			SDL_RenderDrawRect(r, pixelRect);
		}
		else
		{			
			SDL_SetRenderDrawColor(r, static_cast<Uint8>((pixelColor & redflag) >> 16),
				static_cast<Uint8>((pixelColor & greenflag) >> 8), static_cast<Uint8>((pixelColor & blueflag)), 0xffu);
			SDL_RenderFillRect(r, pixelRect);
			if (mainrectFilled && !showingOneMainRect)
			{
				// we colored mainrect pixels by red only if showingOneMainRect is false
				// if we have one big main rect in the screen, we do not 
				// show red border of main rect pixels
				SDL_SetRenderDrawColor(r, RED);
				SDL_RenderDrawRect(r, pixelRect);
			}
		}

	}

    Pixel &Pixel::operator=(const Pixel &p)
    {
#ifdef LOGTOSCREEN
		std::cout << "In pixel copy constr. \n";
#endif
        filled = p.filled;
		mainrectFilled = p.mainrectFilled;
		pixelColor = p.pixelColor;
		positionInColoredArray = p.positionInColoredArray;
		if (pixelRect)
		{
			delete pixelRect; pixelRect = nullptr;
		}
		pixelRect = new Rect;
		pixelRect->x = p.pixelRect->x;
		pixelRect->y = p.pixelRect->y;
		pixelRect->w = p.pixelRect->w;
		pixelRect->h = p.pixelRect->h;
		return *this;
    }

    Pixel::~Pixel()
    {
		delete pixelRect; pixelRect = nullptr;
	}


	CheckedPixel::CheckedPixel(): Pixel ()
	{
		checkPixelRect = new Rect;
	}

	void CheckedPixel::SetPixelData(const pixelData_t& pdata)
	{
		pixelRect->x = pdata.corner.x;
		pixelRect->y = pdata.corner.y;
		pixelRect->w = colorPixelSide;
		pixelRect->h = colorPixelSide;
		pixelColor = pdata.pixelColor;

		checkPixelRect->x = pdata.corner.x - checkPixelSpace;
		checkPixelRect->y = pdata.corner.y - checkPixelSpace;
		checkPixelRect->w = checkPixelRect->h = colorPixelSide +
			checkPixelSpace * 2;
	}

	void CheckedPixel::Draw(SDL_Renderer* const r)
	{
		SDL_SetRenderDrawColor(r, static_cast<Uint8>((pixelColor & redflag) >> 16),
			static_cast<Uint8>((pixelColor & greenflag) >> 8), static_cast<Uint8>((pixelColor & blueflag)), 0xffu);

		SDL_RenderFillRect(r, pixelRect);
		if (checked)
		{
			SDL_SetRenderDrawColor(r, WHITE);
			SDL_RenderDrawRect(r, checkPixelRect);
		}
	}

	CheckedPixel::~CheckedPixel()
	{
		delete checkPixelRect;
		checkPixelRect = nullptr;
	}
#ifdef LOGTOSCREEN
	std::ostream& operator<<(std::ostream& os, const Pixel& p)
	{
		return os << '[' << p.pixelRect->x << ':' << p.pixelRect->y << "]\n";
	}
#endif
}

#undef GREY
#undef WHITE
#undef RED