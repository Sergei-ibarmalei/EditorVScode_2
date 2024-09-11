#pragma once
#include <fstream>
#include "loads.h"
#include "area.h"
#include "sdltype.h"

#include "filedata.h"



namespace ed
{



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
