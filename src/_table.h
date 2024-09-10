#pragma once
#include "pixel.h"
#include "init.h"
#include "flags.h"
#include "const.h"

#ifdef LOGTOSCREEN
#include <iostream>
#endif

namespace ed
{


	class AbstractTable
	{
	protected:
		Rect* tableRowsBoundsArray{ nullptr };
		Rect* tableColsBoundsArray{ nullptr };
		Rect* mainBound{ nullptr };
		int tRows{ 0 };
		int tCols{ 0 };
		virtual bool initiateRowsColsMainBounds() = 0;
		virtual bool makeTable() = 0;
	public:
		//virtual void DrawTable(SDL_Renderer* const r) = 0;
		void DrawBounds(SDL_Renderer* const r);
		bool GetIntersectPosition(SDL_Point& pos, Rect&  mr);
		virtual ~AbstractTable();



	};

	class WorkTable : public AbstractTable, public Init
	{
	private:
		Pixel** workTable{ nullptr };
		pixelData_t* tmpTable{ nullptr };
		virtual bool initiateRowsColsMainBounds() override;
		virtual bool makeTable() override;

		bool 		 makeTableWithData(pixelData_t* dataFromFile);
	public:
		WorkTable(int r = RowMax, int c = ColMax, 
			pixelData_t* dataFromFile = nullptr);
		WorkTable(const WorkTable&) = delete;
		WorkTable& operator=(const WorkTable&) = delete;
		//virtual void DrawTable(SDL_Renderer* const r) override;
		void DrawTable(SDL_Renderer* const r, bool showingOneMainRect = false);
		void SwitchOnOffPixel(const SDL_Point& p, int color = 0);
		void SwitchOnMainRect(const SDL_Point& p);
		void SwitchOffMainRect(const SDL_Point& p);
		void Reset();
		void PixelRememberThis(const SDL_Point& wtablePos, int positionInColoredTable);
		int  PixelRemindThis(const SDL_Point& wtablePos);
		void Rotate(int coloredCount, bool clock = true);
		Pixel** GetWorkTable() const { return workTable; }
		~WorkTable();
	};

	class ColoredTable : public AbstractTable, public Init
	{
	private:
		CheckedPixel** checkedTable{ nullptr };
		SDL_Point baseCheckedPostion{ 0, 0 };
		virtual bool initiateRowsColsMainBounds() override;
		virtual bool makeTable() override;
	public:
		ColoredTable();
		ColoredTable(const ColoredTable&) = delete;
		ColoredTable& operator=(const ColoredTable&) = delete;
		void CheckUncheckColored(const SDL_Point& newCheckPos);
		void Reset();
		//virtual void DrawTable(SDL_Renderer* const r) override;
		void DrawTable(SDL_Renderer* const r);
		~ColoredTable();
	};


}