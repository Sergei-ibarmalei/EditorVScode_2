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
		mainCross = new MainCross;
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
			//if (showingOneMainRect) makeOneMainRect();
			break;
		}
		case SDLK_LEFT:
		{
			wTable->Rotate(countOfFilledPixels + countOfMainRectPixels, false);
			//if (showingOneMainRect) makeOneMainRect();
			break;
		}
		case SDLK_s:
		{
			readyToSaveFile = true; break;
		}

		case SDLK_r:
		{
			// if one main rect is shown then reset flag and exit
			if (showingOneMainRect)
			{
				showingOneMainRect = false;
				break;
			}

			// else make one main rect

			if (countOfMainRectPixels == 0)
			{
				std::cout << "Count of all main rects pixels is less than 1 \n";
				break;
			}
			else makeOneMainRect();

			
			break;
		}

		default: {}
		}
	}

	void Area::Draw()
	{
		wTable->DrawTable(render, showingOneMainRect);
		cTable->DrawTable(render);
		if (showingOneMainRect)
		{
			SDL_SetRenderDrawColor(render, 0xffu, 0, 0, 0xffu);
			SDL_RenderDrawRect(render, &onemainrect);
		}
		mainCross->ShowMainCross(render);

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
		countOfMainRectPixels = 0;
		showingOneMainRect =false;
		onemainrect.x = onemainrect.y = onemainrect.w = onemainrect.h = 0;
	}


	void  Area::makeOneMainRect()
	{
		SDL_Point* upLeftCornersArray = new SDL_Point[countOfMainRectPixels] {0, 0};
		int arrCount {0};
		for (int r = 0; r < workTableRowsCount; r++)
		{
			for (int c = 0; c < workTableColsCount; c++)
			{
				SDL_Rect choosenRect = *wTable->GetWorkTable()[r][c].PixelRect();
				if (wTable->GetWorkTable()[r][c].IsMainRectFilled())
				{
					upLeftCornersArray[arrCount].x = choosenRect.x;
					upLeftCornersArray[arrCount].y = choosenRect.y;
					arrCount += 1;
				}
			}
		}
		onemainrect.x = upLeftCornersArray[0].x;
		onemainrect.y = upLeftCornersArray[0].y;
		onemainrect.w = (upLeftCornersArray[arrCount-1].x + workPixelSide) -
			upLeftCornersArray[0].x;
		onemainrect.h = (upLeftCornersArray[arrCount-1].y + workPixelSide) -
			upLeftCornersArray[0].y;
		
		showingOneMainRect = true;
		delete[] upLeftCornersArray; upLeftCornersArray = nullptr;
	}


	Area::~Area()
	{
		render = nullptr;
		events = nullptr;
		delete wTable; wTable = nullptr;
		delete cTable; cTable = nullptr;
		delete mainCross; mainCross = nullptr;
	}
}


