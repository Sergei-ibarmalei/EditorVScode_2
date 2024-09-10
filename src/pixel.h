#pragma once

#include "rect.h"
#include "flags.h"
#include "const.h"

#ifdef LOGTOSCREEN
#include <iostream>
#endif

typedef struct
{
	SDL_Point corner;
	bool inMainRect;
	int pixelColor;

	// remember the position of pixel's color
	// in colored array (colors.h)
	int posInColoredArray;
}pixelData_t;



namespace ed
{
	class Pixel 
	{
	private:
		bool filled{ false };
		bool mainrectFilled{ false };
	protected:
		Rect* pixelRect{ nullptr };
		int	  pixelColor = 0;
		// recall postion of color in cTable
		// for finding color in cTable by position in wTable
		int positionInColoredArray{ 0 };
		
	public:
		Pixel();
		Pixel(const Pixel&) = delete;
		Pixel& operator=(const Pixel& p);
		~Pixel();
		void SetColor(const int color);
		void SetFill(bool fill) { filled = fill; }
		void SetMainRectFill(bool mrFill) { mainrectFilled = mrFill; }
		void RememberPositionInColoredTable(int posInColoredTable);
		void ForgetPositionInColoredArray() { positionInColoredArray = 0; }
		int  RecallPositionInColoredArray() const { return positionInColoredArray; }
		bool IsFilled() const { return filled; }
		bool IsMainRectFilled() const { return mainrectFilled; }
		int  PixelColorIs() const { return pixelColor; }
		virtual void SetPixelData(const pixelData_t& pdata);
		virtual Rect* PixelRect() { return pixelRect; }
		//virtual void Draw(SDL_Renderer* const r);
		void Draw(SDL_Renderer* const r, bool shoingOneMainRect = false);

#ifdef LOGTOSCREEN
		friend std::ostream& operator<<(std::ostream& os, const Pixel& p);
#endif
	};


	constexpr int checkPixelSpace{ 3 };

	class CheckedPixel : public Pixel
	{
	private:
		Rect* checkPixelRect{ nullptr };
		bool checked{ false };

	public:
		CheckedPixel();
		CheckedPixel(const CheckedPixel&) = delete;
		CheckedPixel& operator=(const CheckedPixel&) = delete;
		~CheckedPixel();

		void SetPixelData(const pixelData_t& pdata) override;
		//void Draw(SDL_Renderer* const r) override;
		void Draw(SDL_Renderer* const r);
		Rect* PixelRect() override { return nullptr; }

		void CheckUncheck() { checked = !checked; }
	};
}