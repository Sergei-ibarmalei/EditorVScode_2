#include "_table.h"
#include "colors.h"

namespace ed
{
	ColoredTable::ColoredTable() : AbstractTable()
	{
		tRows = colorRowsCount;
		tCols = colorColsCount;
		if (!makeTable() || !initiateRowsColsMainBounds())
		{
			Negate(); return;
		}
		// Set check at base position: 0, 0
		checkedTable[baseCheckedPostion.x][baseCheckedPostion.y].CheckUncheck();
		
	}

	bool ColoredTable::initiateRowsColsMainBounds()
	{
		tableRowsBoundsArray = new Rect[tRows];
		tableColsBoundsArray = new Rect[tCols];
		mainBound = new Rect;
		if (!tableRowsBoundsArray || !tableColsBoundsArray || !mainBound)
			return false;

		// initialize colortable cols bounds rects array
		for (int c = 0; c < tCols; c++)
		{
			tableColsBoundsArray[c].x = colorTableStartX + (c * (colorPixelSide +
				colorPixelSpace));
			tableColsBoundsArray[c].y = colorTableStartY;
			tableColsBoundsArray[c].w = colorPixelSide;
			tableColsBoundsArray[c].h = tRows * colorPixelSide +
				(tRows - 1) * colorPixelSpace;
		}
		// initialize colortable rows bounds rects array
		for (int r = 0; r < tRows; r++)
		{
			tableRowsBoundsArray[r].x = colorTableStartX;
			tableRowsBoundsArray[r].y = colorTableStartY + (r * (colorPixelSide +
				colorPixelSpace));
			tableRowsBoundsArray[r].w = tCols * colorPixelSide +
				(tCols - 1) * colorPixelSpace;
			tableRowsBoundsArray[r].h = colorPixelSide;
		}

		// initialize colortable main bound rect
		mainBound->x = colorTableStartX;
		mainBound->y = colorTableStartY;
		mainBound->w = tCols * colorPixelSide +
			(tCols - 1) * colorPixelSpace;
		mainBound->h = tRows * colorPixelSide +
			(tRows - 1) * colorPixelSpace;
		return true;
	}

	bool ColoredTable::makeTable()
	{
		checkedTable = new CheckedPixel * [tRows];
		if (!checkedTable) return false;

		pixelData_t tmpPixel;

		for (int r = 0; r < tRows; r++)
		{

			for (int c = 0; c < tCols; c++)
			{
				checkedTable[r] = new CheckedPixel[tCols];
			}

		}
		for (int r = 0; r < tRows; r++)
		{
			for (int c = 0; c < tCols; c++)
			{
				tmpPixel.corner.x = colorTableStartX + (c * (colorPixelSide +
					colorPixelSpace));
				tmpPixel.corner.y = colorTableStartY + (r * (colorPixelSide +
					colorPixelSpace));
				tmpPixel.pixelColor = colors[(r * tCols) + c];
				checkedTable[r][c].SetPixelData(tmpPixel);
			}
		}
		return true;
	}

	void ColoredTable::DrawTable(SDL_Renderer* const render)
	{
		for (int r = 0; r < tRows; r++)
		{
			for (int c = 0; c < tCols; c++)
			{
				checkedTable[r][c].Draw(render);
			}
		}
	}

	void ColoredTable::CheckUncheckColored(const SDL_Point& newCheckPostion)
	{
		// if old check pos and new one is equal then return
		if ((baseCheckedPostion.x == newCheckPostion.x) &&
			(baseCheckedPostion.y == newCheckPostion.y)) return;
		// delete check at old position
		checkedTable[baseCheckedPostion.x][baseCheckedPostion.y].CheckUncheck();
		baseCheckedPostion.x = newCheckPostion.x;
		baseCheckedPostion.y = newCheckPostion.y;
		checkedTable[baseCheckedPostion.x][baseCheckedPostion.y].CheckUncheck();


	}

	void ColoredTable::Reset()
	{
		checkedTable[baseCheckedPostion.x][baseCheckedPostion.y].CheckUncheck();
		baseCheckedPostion.x = baseCheckedPostion.y = 0;
		checkedTable[baseCheckedPostion.x][baseCheckedPostion.y].CheckUncheck();
	}

	ColoredTable::~ColoredTable()
	{
		if (checkedTable)
		{
			for (int r = 0; r < tRows; r++)
			{
				delete[] checkedTable[r]; checkedTable[r] = nullptr;
			}
			delete[] checkedTable; checkedTable = nullptr;
		}
	}
}