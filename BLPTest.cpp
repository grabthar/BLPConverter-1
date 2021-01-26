
#include "BLPTest.h"

#include "MemImage.h"

const char* pngTestList[] =
{
	"PNG_RGB",
	"PNG_RGBA",
	"PNG_P8",
	"PNG_P8_1Trans",
};

const char* blpTestList[] =
{
	"BLP_U_A0",
	"BLP_U_A1",
	"BLP_U_A8",
	"BLP_DXT1",
	"BLP_DXT1_A1",
	"BLP_DXT3",
	"BLP_DXT5"
};

bool BLPTest::RunTest()
{
	MemImage aMemImage;

	/*
	// PNG->BLP test.
	for (int iFile = 0; iFile < 4; ++iFile)
	{
		char inputFilename[BLPCONV_MAX_PATH];
		::sprintf(inputFilename, "%s.png",  pngTestList[iFile]);
		if (!aMemImage.LoadFromPNG(inputFilename))
			return false;

		for (int iFormat = 1; iFormat < BLPTYPE_COUNT; ++iFormat)
		{
			char outputFilename[BLPCONV_MAX_PATH];
			::sprintf(outputFilename, "%s__%s.blp", pngTestList[iFile], FORMATIDNames[iFormat]);

			if (!aMemImage.SaveToBLP(outputFilename, BLPType(iFormat)))
				return false;
		}
	}

	// BLP->PNG test.
	for (int iFile = 0; iFile < 4; ++iFile)
	{
		char inputFilename[BLPCONV_MAX_PATH];
		::sprintf(inputFilename, "%s.png",  pngTestList[iFile]);
		if (!aMemImage.LoadFromPNG(inputFilename))
			return false;

		for (int iFormat = 1; iFormat < BLPTYPE_COUNT; ++iFormat)
		{
			char outputFilename[BLPCONV_MAX_PATH];
			::sprintf(outputFilename, "%s__%s.blp", pngTestList[iFile], FORMATIDNames[iFormat]);

			if (!aMemImage.SaveToBLP(outputFilename, BLPType(iFormat)))
				return false;
		}
	}
	*/

	return true;
}