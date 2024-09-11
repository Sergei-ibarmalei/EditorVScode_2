#pragma once
#include "init.h"
#include "pixeldata.h"
#include "pixel.h"
#include "const.h"

namespace ed
{
	class FileData : public Init
	{
	private:
		pixelData_t tmpWorkTableLine[workTableRowsCount * workTableColsCount]{};
		int countOfFilled{ 0 };
		int countOfMainRects{ 0 };
	public:
	
		FileData(Pixel** workTable);    // for save data
		FileData() {};                  // for reading data

		FileData(const FileData&) = delete;
		FileData& operator=(const FileData&) = delete;
		~FileData() = default;
		void SetCountOfFilled(int count) { countOfFilled = count; }
		void SetCountOfMainRects(int count) { countOfMainRects = count; }
		int  GetCountOfFilled() const { return countOfFilled; }
		int  GetCountOfMainRects() const { return countOfMainRects; }
		pixelData_t* GetWorkTableLine()  { return tmpWorkTableLine; }
		
	};
}