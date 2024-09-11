#include "_table.h"

namespace ed
{
	AbstractTable::~AbstractTable()
	{
		delete tableRowsBoundsArray; tableRowsBoundsArray = nullptr;
		delete tableColsBoundsArray; tableColsBoundsArray = nullptr;
		delete mainBound; mainBound = nullptr;
	}

	void AbstractTable::DrawBounds(SDL_Renderer* const render)
	{
		SDL_SetRenderDrawColor(render, 0xffu, 0u, 0u, 0xffu);
		for (int r = 0; r < tRows; r++)
		{
			SDL_RenderDrawRect(render, &tableRowsBoundsArray[r]);
		}
		for (int c = 0; c < tCols; c++)
		{
			SDL_RenderDrawRect(render, &tableColsBoundsArray[c]);
		}
		SDL_RenderDrawRect(render, mainBound);
	}


	/// <summary>
	/// Compute row and column of workTable or coloredTable
	/// 
	/// </summary>
	/// <param name="pos"> computed position </param>
	/// <param name="mr"> mouse rect for computation </param>
	/// <returns> true if mouse rect is in workTable or coloredTable</returns>
	bool AbstractTable::GetIntersectPosition(SDL_Point& pos, 
			Rect&  mr)
	{

		int up{ 0 };
		int left{ 0 };
		int down{ tRows - 1 };
		int right{ tCols - 1 };
		bool flag{ false };
		int middleRow = 0;
		int middleCol = 0;

		//Uint32 buttonStatus = SDL_GetMouseState(&mr.x, &mr.y);

		if (!(mr == mainBound)) return false;

		while ((up <= down) && !flag)
		{
			middleRow = (up + down) >> 1;
			if (mr * tableRowsBoundsArray[middleRow]) flag = true;
			if ((mr.y + mr.h) < tableRowsBoundsArray[middleRow].y)
				down = middleRow - 1;
			else up = middleRow + 1;
		}
		if (!flag) return false;
		flag = false;
		while ((left <= right) && !flag)
		{
			middleCol = (left + right) >> 1;
			if (mr * tableColsBoundsArray[middleCol]) flag = true;
			if ((mr.x + mr.w) < tableColsBoundsArray[middleCol].x)
				right = middleCol - 1;
			else left = middleCol + 1;
		}

		pos.x = middleRow;
		pos.y = middleCol;

		return true;
	}




	WorkTable::WorkTable(int r, int c,
		pixelData_t* dataFromFile): AbstractTable()
	{

		tRows = r;
		tCols = c;
		if (tRows <= 0 || tCols <= 0)
		{
			tRows = RowMin; tCols = ColMin;
		}
		if (tRows >= RowMax || tCols >= ColMax)
		{
			tRows = RowMax; tCols = ColMax;
		}

		// if we sent a loaded data from some file:
		if (dataFromFile)
		{
			if (!makeTableWithData(dataFromFile) || !initiateRowsColsMainBounds())
			{
				Negate();
#ifdef LOGTOSCREEN
				std::cout << "Cannot make table with data, abort.\n";
#endif
			}
			return;
		}
		if (!makeTable() || !initiateRowsColsMainBounds()) Negate();




	}


	bool WorkTable::makeTableWithData(pixelData_t* dataFromFile)
	{
		if (!dataFromFile)
		{
			std::cout << "Data from file is absent, abort\n"; 
			return false;
		}
		workTable = new Pixel * [tRows] {nullptr};
		if (!workTable)
		{
#ifdef LOGTOSCREEN
			std::cout << "Memory allocation is failed.\n";
#endif
			return false;

		}
		for (int r = 0; r < tRows; r++)
		{
			workTable[r] = new Pixel[tCols];
		}

		// Copying data from dataFromFile to workTable:
#ifdef LOGTOSCREEN
		std::cout << "Copying data from file\n";
#endif
		for (int r = 0; r < tRows; r++)
		{
			for (int c = 0; c < tCols; c++)
			{
				workTable[r][c].SetPixelData(dataFromFile[(r * workTableColsCount) + c]);
			}
		}
		return true;
	}

	bool WorkTable::initiateRowsColsMainBounds()
	{
		tableRowsBoundsArray = new Rect[tRows];
		tableColsBoundsArray = new Rect[tCols];
		mainBound = new Rect;
		if (!tableRowsBoundsArray || !tableColsBoundsArray || !mainBound)
			return false;

		for (int r = 0; r < tRows; r++)
		{
			tableRowsBoundsArray[r].x = workTableStartX;
			tableRowsBoundsArray[r].y = workTableStartY + (r * (workPixelSide +
				workPixelSpace));
			tableRowsBoundsArray[r].w = tCols * workPixelSide +
				(tCols - 1) * workPixelSpace;
			tableRowsBoundsArray[r].h = workPixelSide;
		}

		for (int c = 0; c < tCols; c++)
		{
			tableColsBoundsArray[c].x = workTableStartX + (c * (workPixelSide +
				workPixelSpace));
			tableColsBoundsArray[c].y = workTableStartY;
			tableColsBoundsArray[c].w = workPixelSide;
			tableColsBoundsArray[c].h = tRows * workPixelSide +
				(tRows - 1) * workPixelSpace;
		}

		mainBound->x = workTableStartX;
		mainBound->y = workTableStartY;
		mainBound->w = tCols * workPixelSide +
			(tCols - 1) * workPixelSpace;
		mainBound->h = tRows * workPixelSide +
			(tRows - 1) * workPixelSpace;
		
		return true;
	}

	bool WorkTable::makeTable()
	{
		workTable = new Pixel * [tRows];
		if (!workTable) return false;

		pixelData_t tmpPixel;
		tmpPixel.pixelColor = -1; // without color
		tmpPixel.inMainRect = false;

		for (int r = 0; r < tRows; r++)
		{
			workTable[r] = new Pixel[tCols];
		}
		for (int r = 0; r < tRows; r++)
		{
			for (int c = 0; c < tCols; c++)
			{
				tmpPixel.corner.x = workTableStartX + (c * (workPixelSide +
					workPixelSpace));
				tmpPixel.corner.y = workTableStartY + (r * (workPixelSide +
					workPixelSpace));
				workTable[r][c].SetPixelData(tmpPixel);
			}
		}
		return true;
	}

	void WorkTable::DrawTable(SDL_Renderer* const render, bool showingOneMainRect)
	{
		for (int r = 0; r < tRows; r++)
		{
			for (int c = 0; c < tCols; c++)
			{
				workTable[r][c].Draw(render, showingOneMainRect);
			}
		}
	}

	// make pixel at position wtablePos to remember position of choosen color
	// in colored table 
	void WorkTable::PixelRememberThis(const SDL_Point& wtablePos, int postionInColoredTable)
	{
		workTable[wtablePos.x][wtablePos.y].RememberPositionInColoredTable(postionInColoredTable);
	}

	int WorkTable::PixelRemindThis(const SDL_Point& wtablePos)
	{
		return workTable[wtablePos.x][wtablePos.y].RecallPositionInColoredArray();
	}



	void WorkTable::SwitchOnOffPixel(const SDL_Point& p, int color)
	{

		if (color)
		{
			workTable[p.x][p.y].SetColor(color);
			workTable[p.x][p.y].SetFill(true);
		}
		else
		{
			workTable[p.x][p.y].SetFill(false);
		}


	}

	void WorkTable::SwitchOnMainRect(const SDL_Point& p)
	{
		if (workTable[p.x][p.y].IsMainRectFilled()) return;
		workTable[p.x][p.y].SetMainRectFill(true);
	}

	void WorkTable::SwitchOffMainRect(const SDL_Point& p)
	{
		if (!workTable[p.x][p.y].IsMainRectFilled()) return;
		workTable[p.x][p.y].SetMainRectFill(false);
	}

	void WorkTable::SwitchOnFire(const SDL_Point& p)
	{
		if (workTable[p.x][p.y].IsFire()) return;
		workTable[p.x][p.y].SetFire(true);
	}

	void WorkTable::SwitchOffFire(const SDL_Point& p)
	{
		if (!workTable[p.x][p.y].IsFire()) return;
		workTable[p.x][p.y].SetFire(false);
	}

	void WorkTable::Reset()
	{
		for (int r = 0; r < tRows; r++)
		{
			for (int c = 0; c < tCols; c++)
			{
				workTable[r][c].SetFill(false);
				workTable[r][c].SetColor(-1);
				workTable[r][c].ForgetPositionInColoredArray();
				workTable[r][c].SetMainRectFill(false);
				workTable[r][c].SetFire(false);
			}
		}
	}



	// coloredCount = count of colored pixels in wTable
	void WorkTable::Rotate(int coloredMainRectsFireCount, bool clock)
	{
		if (!coloredMainRectsFireCount) return;
		tmpTable = new pixelData_t[coloredMainRectsFireCount];
		// in this array we store positions in color array of pixel
		// when we rotate table we must remember it
		int* tmpReminderColoredPos = new int[coloredMainRectsFireCount] {0};

		int tmpTableIter = 0;
#define ROW corner.y
#define COL corner.x

		for (int r = 0; r < tRows; r++)
		{
			for (int c = 0; c < tCols; c++)
			{
				// if pixel in wTable is filled by some color:
				// and need to rotate clockwise

				if ((workTable[r][c].IsFilled() || workTable[r][c].IsMainRectFilled() || workTable[r][c].IsFire()) && clock)
				{
					// remember the position of that pixel
					tmpTable[tmpTableIter].ROW = tRows - r - 1;
					tmpTable[tmpTableIter].COL = c;	
					tmpTable[tmpTableIter].pixelColor = workTable[r][c].PixelColorIs();
					// do remember if pixel is in red main rect
					tmpTable[tmpTableIter].inMainRect = workTable[r][c].IsMainRectFilled();					
					tmpReminderColoredPos[tmpTableIter] = workTable[r][c].RecallPositionInColoredArray();
					tmpTable[tmpTableIter].isFire = workTable[r][c].IsFire();
					tmpTableIter += 1;
					
				}
				// and need to rotate counterclockwise

				else if ((workTable[r][c].IsFilled() || workTable[r][c].IsMainRectFilled() || workTable[r][c].IsFire()) && !clock)
				{
					// remember the position of that pixel
					tmpTable[tmpTableIter].COL = tCols - c - 1;
					tmpTable[tmpTableIter].ROW = r;
					tmpTable[tmpTableIter].pixelColor = workTable[r][c].PixelColorIs();
					tmpTable[tmpTableIter].inMainRect = workTable[r][c].IsMainRectFilled();
					tmpReminderColoredPos[tmpTableIter] = workTable[r][c].RecallPositionInColoredArray();
					tmpTable[tmpTableIter].isFire = workTable[r][c].IsFire();
					tmpTableIter += 1;
				}

			}
		}


		this->Reset();
		for (int i = 0; i < tmpTableIter; i++)
		{
			if (tmpTable[i].pixelColor != -1)
			{
				workTable[tmpTable[i].COL][tmpTable[i].ROW].SetFill(true);
				workTable[tmpTable[i].COL][tmpTable[i].ROW].SetColor(tmpTable[i].pixelColor);
			}
			workTable[tmpTable[i].COL][tmpTable[i].ROW].RememberPositionInColoredTable(tmpReminderColoredPos[i]);			
			// recall is pixel in red main rect
			workTable[tmpTable[i].COL][tmpTable[i].ROW].SetMainRectFill(tmpTable[i].inMainRect);	
			workTable[tmpTable[i].COL][tmpTable[i].ROW].SetFire(tmpTable[i].isFire);	
		}

		delete[] tmpTable; tmpTable = nullptr;
		delete[] tmpReminderColoredPos; tmpReminderColoredPos = nullptr;
#undef ROW
#undef COL
	}


	WorkTable::~WorkTable()
	{
		if (workTable)
		{
			for (int r = 0; r < tRows; r++)
			{
				delete[] workTable[r]; workTable[r] = nullptr;
			}
			delete[] workTable; workTable = nullptr;
		}
	}
 
}