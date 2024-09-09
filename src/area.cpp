#include "area.h"
constexpr int currentWorkTableRows{ 16 };
constexpr int currentWorkTableCols{ 16 };

namespace ed
{
	Area::Area(SDL_Renderer* const r,  SDL_Event* e, pixelData_t* dataFromFile)
	{
		if (!r)
		{
			Negate(); return;
		}
		render = r;
		events = e;
		wTable = new WorkTable(currentWorkTableRows, currentWorkTableCols, dataFromFile);
		cTable = new ColoredTable;
	}

	void Area::CheckKeys(SDL_Event& e)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_c:
		{
			reset(); break;
		}
		case SDLK_RIGHT:
		{
			wTable->Rotate(countOfFilledPixels + countOfMainRectPixels);
			break;
		}
		case SDLK_LEFT:
		{
			wTable->Rotate(countOfFilledPixels + countOfMainRectPixels, false);
			break;
		}
		case SDLK_s:
		{
			readyToSaveFile = true; break;
		}

		default: {}
		}
	}

	void Area::Draw()
	{
		wTable->DrawTable(render);
		cTable->DrawTable(render);

	}

	void Area::TestDrawbounds()
	{
		wTable->DrawBounds(render);
		cTable->DrawBounds(render);
	}

	void Area::ChecksPos(const Uint8* keyState)
	{


		if (events->type == SDL_MOUSEBUTTONDOWN)
		{
			Uint32 buttonStatus = SDL_GetMouseState(&mouseRect.x, &mouseRect.y);
			bool intersectInWorkTable = wTable->GetIntersectPosition(wFoundPixel, mouseRect);
			bool intersectInColorTable = cTable->GetIntersectPosition(cFoundPixel, mouseRect);

			// if right mouse click: work with workTable
			if (buttonStatus & SDL_BUTTON(SDL_BUTTON_RIGHT))
			{

				if (intersectInWorkTable)
				{
					// Erase color in pixel
					wTable->SwitchOnOffPixel(wFoundPixel);
					if (countOfFilledPixels > 0) countOfFilledPixels = -1;
				}
				return;
			}
			if (buttonStatus & SDL_BUTTON(SDL_BUTTON_MIDDLE))
			{
				// middle mouse button works for finding color in 
				// colored table
				if (intersectInWorkTable)
				{

					int indexInColoredArray = wTable->PixelRemindThis(wFoundPixel);
					int row = indexInColoredArray / colorColsCount;
					int col = indexInColoredArray % colorColsCount;
#ifdef LOGTOSCREEN
					std::cout << "Color: 0x" << std::hex << 
						colors[indexInColoredArray] << std::dec << std::endl;
#endif
					choosenColor = colors[indexInColoredArray];
					cTable->CheckUncheckColored({ (row == 0 ? 0 : row), col });
					return;
				}
			}
			// if left click: work with colorTable
			if (intersectInColorTable)
			{
				// compute new color
				int indexInColoredArray = (cFoundPixel.x * colorColsCount) + cFoundPixel.y;
				choosenColor = colors[indexInColoredArray];
				// set check at new position
				cTable->CheckUncheckColored(cFoundPixel);
			}
			// if left click: work with workTable
			if (intersectInWorkTable)
			{
				if (keyState[SDL_SCANCODE_LSHIFT])
				{
					wTable->SwitchOnMainRect(wFoundPixel); 
					//==
					countOfMainRectPixels += 1;
					//==
					return;
				}
				if (keyState[SDL_SCANCODE_RSHIFT])
				{
					wTable->SwitchOffMainRect(wFoundPixel); 
					//==
					countOfMainRectPixels -= 1;
					if (countOfMainRectPixels < 0) countOfMainRectPixels = 0;
					//==
					return;
				}

				int indexInColoredArray = (cFoundPixel.x * colorColsCount) + cFoundPixel.y;
				wTable->SwitchOnOffPixel(wFoundPixel, choosenColor);
				countOfFilledPixels += 1;
				// Asking pixel that was found by left clicking mouse to
				// remember an index of colored array
				// In future we would need to find this color again
				wTable->PixelRememberThis(wFoundPixel, indexInColoredArray);
			}

		}




	}

	void Area::reset()
	{
		wTable->Reset();
		cTable->Reset();
		choosenColor = colors[0];
		countOfFilledPixels = 0;
		wFoundPixel = cFoundPixel = { 0, 0 };
	}


	Area::~Area()
	{
		render = nullptr;
		events = nullptr;
		delete wTable; wTable = nullptr;
		delete cTable; cTable = nullptr;
	}
}