#include "editor.h"
#define BLACK 0u, 0u, 0u, 0xffu
#define NOFILEDATA nullptr

// for save data
ed::FileData::FileData(Pixel** workTable)
{
	if (!workTable)
	{
		Negate();
#ifdef LOGTOSCREEN
		std::cout << "Cannot make File Data, work table is absent, abort.\n";
#endif
		return;
	}

	for (int r = 0; r < workTableRowsCount; r++)
	{
		for (int c = 0; c < workTableColsCount; c++)
		{
			auto atNomber = (r * workTableRowsCount) + c;
			tmpWorkTableLine[atNomber].corner.x =
				workTable[r][c].PixelRect()->x;
			tmpWorkTableLine[atNomber].corner.y =
				workTable[r][c].PixelRect()->y;
			tmpWorkTableLine[atNomber].inMainRect =
				workTable[r][c].IsMainRectFilled();
			tmpWorkTableLine[atNomber].pixelColor =
				workTable[r][c].PixelColorIs();
			tmpWorkTableLine[atNomber].posInColoredArray =
				workTable[r][c].RecallPositionInColoredArray();
		}
	}
}


ed::Editor::Editor(int argc_, char** argv_)
{
	loads = new Loads(argc_, argv_);
	if (!loads->InitOk())
	{
		Negate(); return;
	}
	sdl = new SDL("Editor");
	if (!sdl->InitOk())
	{
		Negate(); return;
	}
	EditorMode editorMode = loads->GetMode();
	if (editorMode == EditorMode::loadProject)
	{
		if (readFileProcess())
		{
			area = new Area(sdl->Render(), &sdl->Event(), fd->GetWorkTableLine());
			area->SetCountOfFilledPixels(fd->GetCountOfFilled());
			area->SetCountOfMainRectPixels(fd->GetCountOfMainRects());
			if (!area->InitOk()) Negate();
		} 
		else Negate();
	}
	else
	{
		area = new Area(sdl->Render(), &sdl->Event(), NOFILEDATA);
		if (!area->InitOk())
		{
			Negate(); return;
		}
	}
#undef NOFILEDATA

}

ed::Editor::~Editor()
{
	delete loads; loads = nullptr;
	delete sdl; sdl = nullptr;
	delete area; area = nullptr;
	if (fd)
	{
		delete fd; fd = nullptr;
	}
}

void ed::Editor::MainLoop()
{
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&sdl->Event()) != 0)
		{
			if (sdl->Event().type == SDL_QUIT) quit = true;
			else if (sdl->Event().type == SDL_KEYDOWN)
			{
				area->CheckKeys(sdl->Event());
			}
			// if S key pressed and process of saving file is not going
			if (area->ReadyToSaveFile() && !saveFileProcessIsGoing)
			{
#ifdef LOGTOSCREEN
				std::cout << "Begin to save file process.\n";
#endif
				saveFileProcessIsGoing = true;
				if (!saveFileProcess()) return;
			}


			const Uint8* currentKeyState = SDL_GetKeyboardState(NULL);
			area->ChecksPos(currentKeyState);
		}

		SDL_SetRenderDrawColor(sdl->Render(), BLACK);
		SDL_RenderClear(sdl->Render());
		area->Draw();
		SDL_RenderPresent(sdl->Render());
	}
}

bool ed::Editor::readFileProcess()
{
	const char* nameToRead = loads->GetFileName();
	if (!nameToRead)
	{
#ifdef LOGTOSCREEN
		std::cout << "File name is absent, abort.\n";
#endif
		return false;
	}
	std::streamsize fileSize = static_cast<std::streamsize>(sizeof(FileData));
	std::ifstream fileToRead(nameToRead, std::ios_base::in |
		std::ios_base::binary);
	fd = new FileData;
	fileToRead.read((char*)fd, fileSize);
	if (!fileToRead.is_open())
	{
#ifdef LOGTOSCREEN
		std::cout << "Cannot open file, abort.\n";
#endif
		fileToRead.close();
		return false;
	}
	fileToRead.close();
	return true;
}


bool ed::Editor::saveFileProcess()
{
	const char* nameToWrite = loads->GetFileName();
	if (fd)
	{
		delete fd; fd = nullptr;
	}
	fd = new FileData(area->GetWorkTable());
	if (fd->InitOk() == false)
	{
#ifdef LOGTOSCREEN
		std::cout << "Cannot make save process, abort.\n";
#endif
		return false;
	}
	fd->SetCountOfFilled(area->CountOfFilledPixels());
	fd->SetCountOfMainRects(area->CountOfMainRectPixels());
	std::streamsize filesize = static_cast<std::streamsize>(sizeof(*fd));
	std::ofstream fileOut(nameToWrite, std::ios_base::out |
		std::ios_base::trunc | std::ios_base::binary);
	fileOut.write((char*) fd, filesize);
	fileOut.close();
	saveFileProcessIsGoing = false;
	area->ResetSaveFileProcessFlag();
	return true;
}





#undef BLACK
