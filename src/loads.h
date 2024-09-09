#pragma once
#include <iostream>
#include <fstream>
#include <string.h>
#include "init.h"
#include "flags.h"


enum class EditorMode
{
	newProject,
	loadProject,
	failure
};

constexpr int bufferlength {16};

namespace ed
{
	class Loads : public Init
	{
	private:
		int argc{ 0 };
		char** fileName {nullptr};
		char buffer[bufferlength] {0};
		EditorMode editorMode{ EditorMode::newProject };

		EditorMode getEditorMode(char** argv);
		bool checkFileName();
		
	public:
		Loads(int argc_, char** argv_);
		Loads(const Loads&) = delete;
		Loads& operator=(const Loads&) = delete;
		~Loads();

		const char* GetFileName() const {return &buffer[0];}
		EditorMode GetMode() const {return editorMode;}

		
	};
}