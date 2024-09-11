#pragma once


#include "_table.h"
#include "colors.h"
#include "const.h"
#include "maincross.h"



namespace ed
{

	class Area : public Init
	{
	private:
		WorkTable* wTable{ nullptr };
		ColoredTable* cTable{ nullptr };
		SDL_Renderer* render{ nullptr };
		SDL_Event* events{ nullptr };
		MainCross* mainCross {nullptr};
		Rect mouseRect{ 0, 0, 1,1 };
		// For show main rect
		Rect onemainrect{0, 0, 0, 0};
		SDL_Point wFoundPixel{ 0, 0 };
		SDL_Point cFoundPixel{ 0, 0 };
		int choosenColor{ colors[0] };
		bool showingOneMainRect {false};

		int countOfFilledPixels{ 0 };
		int countOfMainRectPixels{ 0 };
		int countOfFires{0};

		// is S key pressed?
		bool readyToSaveFile {false};
		void reset();
		void  makeOneMainRect();

	public:
		Area(SDL_Renderer* const r, SDL_Event* e, pixelData_t* dataFromFile = nullptr);
		Area(const Area&) = delete;
		Area& operator=(const Area&) = delete;
		~Area();

		void CheckKeys(SDL_Event& e);
		void Draw();
		void TestDrawbounds();
		void ChecksPos(const Uint8* keyState);


		int CountOfFilledPixels() const { return countOfFilledPixels; }
		int CountOfMainRectPixels() const { return countOfMainRectPixels; }

		bool ReadyToSaveFile() const {return readyToSaveFile;}
		void ResetSaveFileProcessFlag() {readyToSaveFile = false;}

		Pixel** GetWorkTable() const {return wTable->GetWorkTable();}
		void  SetCountOfFilledPixels(int count) { countOfFilledPixels = count; }
		void  SetCountOfMainRectPixels(int count) { countOfMainRectPixels = count; }
	};
}

