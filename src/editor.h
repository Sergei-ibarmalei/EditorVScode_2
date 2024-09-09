#pragma once
#include <fstream>
#include "loads.h"
#include "area.h"
#include "sdltype.h"




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


	class Editor : public Init
	{
	private:
		Loads* loads{ nullptr };
		SDL* sdl{ nullptr };
		Area* area{ nullptr };
		FileData* fd{ nullptr };
		
		bool saveFileProcessIsGoing{false};
		bool saveFileProcess();
		bool readFileProcess();
		
		
	public:
		Editor(int argc_, char** argv_);
		Editor(const Editor&) = delete;
		Editor& operator=(const Editor&) = delete;
		~Editor();

		void MainLoop();

	};
}
