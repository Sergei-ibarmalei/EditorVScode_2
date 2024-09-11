#include "filedata.h"

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