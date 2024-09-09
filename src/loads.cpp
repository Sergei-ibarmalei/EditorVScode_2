#include "loads.h"

namespace ed
{
	Loads::Loads(int argc_, char** argv_)
	{

		if (argc_ == 1)
		{
			std::cout << "Run with parameter <new> <filename> for new " <<
				"project or " <<
				"<load> <filename> for load project. \n";
			Negate(); return;
		}
		argc = argc_;
		fileName = argv_;
		editorMode = getEditorMode(argv_);
		if (editorMode == EditorMode::failure)
		{
			Negate(); return;
		}
		if (!checkFileName())
		{
#ifdef LOGTOSCREEN
			std::cout << "Wrong file name, abort.\n";
#endif
			Negate(); return;
		}
		else
		{
#ifdef LOGTOSCREEN
			std::cout << "Project file name is: " << buffer << std::endl;
#endif
			
		}

	}

	bool Loads::checkFileName()
	{
    	int iter = 0;
		
    	while (fileName[2][iter])
    	{
			buffer[iter] = fileName[2][iter];
        	switch (buffer[iter])
        	{
            	case ':': return false;
            	case '<': return false;
            	case '>': return false;
            	case '/': return false;
            	case 92:  return false;
            	case '*': return false;
            	case '?': return false;
            	case '.': 
            	{
                	buffer[iter] = '\0'; return true;
            	}
            	default: {}
        	}
        	if (iter == (bufferlength-1))
        	{
            	buffer[iter] = fileName[2][iter];
            	buffer[iter+1] = '\0';
            	return true;
        	}
        	iter += 1;
    	}
    	return true;
	}


	EditorMode Loads::getEditorMode(char** argv)
	{
		if (strcmp(argv[1], "new") == 0)
		{
			return EditorMode::newProject;
			
		}
		if (strcmp(argv[1], "load") == 0)
		{
			return EditorMode::loadProject;
		}
		else
		{
			std::cout << "Run with parameter <new> <filename>" <<
			" for new project or " <<
			"<load> <filename> for load project. \n";
			Negate();			
		}
		return EditorMode::failure;
	}


	Loads::~Loads()
	{
		fileName = nullptr;
	}
}