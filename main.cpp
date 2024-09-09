
#include "src/editor.h"

#undef main

int main(int argc, char** argv)
{

	ed::Editor editor(argc, argv);
	if (!editor.InitOk())
	{
#ifdef LOGTOSCREEN
		std::cout << "Something went wrong.\n";
#endif
		return -1;
	}
	editor.MainLoop();
	return 0;
}