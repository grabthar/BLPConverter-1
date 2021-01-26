//	2008 Patrick Cyr (kobuster@yahoo.com)
//	2020 Grabthar

#include "BLPConverter.h"
#include "BLPTest.h"
#include "MemImage.h"
#include "Log.h"

#include "png.h"
#include "squish/squish.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>


#ifndef LINUX
#include <windows.h>
#else
#include "port.h"
#include <libgen.h>
#include <sys/types.h>
#include <dirent.h>
#include <fnmatch.h>
#include <errno.h>
#include "palbmp/palcreate.h"
#include "palbmp/palettize.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// User options.
bool g_bInfoMode = false;
FORMATID g_targetFormatID = FORMAT_UNSPECIFIED;


int ProcessFile(const char* pszFilenameArgument, const char* pszDestinationFilename)
{
	char pszFilenameBuffer[MAX_PATH];
	strcpy(pszFilenameBuffer, pszFilenameArgument);

	char* pszPeriod = ::strrchr(pszFilenameBuffer, '.');
	if (NULL == pszPeriod)
	{
		printf("Invalid filename '%s' (no . in it).\n", pszFilenameArgument);
		return - 1;
	}
	pszPeriod[0] = 0;
	char* pszExtension = &pszPeriod[1];
	
	// Load the file.
	FileType inputFileType;
	FORMATID inputFormatID;
	MemImage aMemImage;
	if (0 == ::_stricmp(pszExtension, "blp"))
	{
		inputFileType = FILETYPE_BLP;
		LoadResult result = aMemImage.LoadFromBLP(pszFilenameArgument, &inputFormatID);
		if (result != LoadResult::LOADRESULT_SUCCESS)
		{
			return int(result);
		}
	}
	else if (0 == ::_stricmp(pszExtension, "png"))
	{
		inputFileType = FILETYPE_PNG;
		if (!aMemImage.LoadFromPNG(pszFilenameArgument, &inputFormatID))
		{
			return -1;
		}
	}
	else
	{
		printf("ERROR: Input file '%s' not a png or blp.\n", pszFilenameArgument);
		return -1;
	}

	if (g_bInfoMode)
		return 0;

	// Look up the target format.
	FORMATID targetFormatID = g_targetFormatID;
	if (FORMAT_UNSPECIFIED == targetFormatID)
		targetFormatID = MemImage::s_ruleTable[inputFormatID];

	// Generate the target filename.
	char pszTargetFilename[MAX_PATH];
	if (NULL == pszDestinationFilename)
	{
		bool bSameFiletype = (FILETYPE_BLP == inputFileType && ISBLP(targetFormatID)) || (FILETYPE_PNG == inputFileType && ISPNG(targetFormatID));
		::sprintf(pszTargetFilename, "%s%s.%s", (char*) pszFilenameBuffer, bSameFiletype ? "_" : "", ISBLP(targetFormatID) ? "blp" : "png");
	}
	else
		::sprintf(pszTargetFilename, pszDestinationFilename);

	// Save the file with the given format.
	LOG("Converting: %s (%s) -> %s (%s)\n", pszFilenameArgument, FORMATIDNames[inputFormatID], pszTargetFilename, FORMATIDNames[targetFormatID]);
	bool result = aMemImage.Save(pszTargetFilename, targetFormatID);

	return result ? 0 : -1;
}

int main(int argc, char* argv[])
{
	WriteLog("Started");

	int result = 0;

	const char* pszSourceFilename = NULL;
	const char* pszDestFilename = NULL;
	bool bMultiFileMode = false;
	bool bPauseAtEnd = false;
	bool bPauseOnError = false;
	bool bRunTest = false;
	bool bClothingOptionSet = false;
	bool bCharacterOptionSet = false;

	if (argc == 1)
	{
		WriteUsage();
		result = -1;
		goto Finish;
	}
	
	int iArg;
	for (iArg = 1; iArg < argc; ++iArg)
	{
		if ('-' == argv[iArg][0] || '/' == argv[iArg][0])
		{
			switch (argv[iArg][1])
			{
			case '?':
				WriteUsage();
				result = 0;
				goto Finish;
				break;
			case 'a':
			case 'A':
			{
				int threshold = atoi(&argv[iArg][2]);
				if (threshold < 0 || threshold > 255)
				{
					printf("ERROR: Alpha threshold must be between 0 and 255.\n");
					result = -1;
					goto Finish;
				}
				MemImage::s_byAlphaThreshold = threshold;
				break;
			}
			case 'c':
			case 'C':
				MemImage::s_bCreateMipTestImage = true;
				break;
			case 'e':
			case 'E':
				bPauseOnError = true;
				break;
			case 'f':
			case 'F':
			{
				const char* arg = &argv[iArg][2];
				g_targetFormatID = FORMAT_UNSPECIFIED;

				g_targetFormatID = GetFormatFromString(arg);
				if (FORMAT_UNSPECIFIED == g_targetFormatID)
				{
					ListFormats();
					result = -1;
					goto Finish;
				}

				break;
			}
			/*
			case 'g':
			case 'G':
				MemImage::s_fGammaFactor = (float) atof(&argv[iArg][2]);
				if (MemImage::s_fGammaFactor < 0.0f || MemImage::s_fGammaFactor > 1.0f)
				{
					printf("ERROR: Gamma factor must between 0.0 and 1.0.\n");
					result = -1;
					goto Finish;
				}
				break;
			*/
			case 'h':
			case 'H':
				bCharacterOptionSet = true;
				MemImage::s_ruleTable[PNGTYPE_PALETTIZED_ALPHAMASK] = BLPTYPE_PAL_ALPHA0;
				MemImage::s_ruleTable[PNGTYPE_RGB] = BLPTYPE_PAL_ALPHA0;
				MemImage::s_ruleTable[PNGTYPE_RGBA] = BLPTYPE_PAL_ALPHA0;
				break;
			case 'i':
			case 'I':
				printf("File Info Mode.\n");
				g_bInfoMode = true;
				MemImage::s_bVerbose = true;
				bMultiFileMode = true;
				break;
			case 'l':
			case 'L':
				ListFormats();
				goto Finish;
				break;
			case 'm':
			case 'M':
				bMultiFileMode = true;
				break;
			case 'n':
			case 'N':
				MemImage::s_bNoMips = true;
				break;
			case 'p':
			case 'P':
				bPauseAtEnd = true;
				break;
			case 'r':
			case 'R':
				bClothingOptionSet = true;
				MemImage::s_ruleTable[PNGTYPE_RGB] = BLPTYPE_PAL_ALPHA0;
				MemImage::s_ruleTable[PNGTYPE_RGBA] = BLPTYPE_PAL_ALPHA8;
				MemImage::s_ruleTable[BLPTYPE_PAL_ALPHA0] = PNGTYPE_RGB;
				MemImage::s_ruleTable[BLPTYPE_PAL_ALPHA1] = PNGTYPE_RGBA;
				MemImage::s_ruleTable[BLPTYPE_PAL_ALPHA4] = PNGTYPE_RGBA;
				MemImage::s_ruleTable[BLPTYPE_PAL_ALPHA8] = PNGTYPE_RGBA;
				break;
			case 't':
			case 'T':
				bRunTest = true;
				break;
			case 'u':
			case 'U':
			{
				char arg[128];
				::strncpy(arg, &argv[iArg][2], 127);
				char* pszGT = ::strchr(arg, '=');
				if (NULL == pszGT)
				{
					LOG("ERROR: No \"=\" character found in /U option.\n");
					result = -1;
					goto Finish;
				}
				pszGT[0] = 0;
				char* arg2 = &pszGT[1];
				FORMATID srcID = GetFormatFromString(arg);
				FORMATID destID = GetFormatFromString(arg2);

				if (FORMAT_UNSPECIFIED == srcID || FORMAT_UNSPECIFIED == destID)
				{
					ListFormats();
					result = -1;
					goto Finish;
				}
				
				MemImage::s_ruleTable[srcID] = destID;
				break;
			}
			case 'v':
			case 'V':
				MemImage::s_bVerbose = true;
				break;
			default:
				printf("ERROR; %c is not a valid option.\n", argv[iArg][1]);
				result = -1;
				goto Finish;
			}
		}
		else
		{
			break;
		}
	}

	if (bRunTest)
	{
		if (!BLPTest::RunTest())
		{
			result = -1;
		}
		goto Finish;
	}

	if (bClothingOptionSet && bCharacterOptionSet)
	{
		printf("ERROR: -r and -h are exclusive.\n");
		result = -1;
		goto Finish;
	}

	if (iArg == argc)
	{
		printf("ERROR: Filename argument missing.\n");
		result = -1;
		goto Finish;
	}
	else if (bMultiFileMode)
	{
		do
		{
#ifndef LINUX
			char filepath[BLPCONV_MAX_PATH] = "";
			::strcpy(filepath, argv[iArg]);
			char* slash = ::strrchr(filepath, '\\');
			if (NULL == slash)
				slash = ::strrchr(filepath, '/');
			if (NULL != slash)
				slash[1] = 0;
			else
				filepath[0] = 0;

			WIN32_FIND_DATA aFindData;
			HANDLE hFindFile = ::FindFirstFile(argv[iArg], &aFindData);
			if (INVALID_HANDLE_VALUE == hFindFile)
			{
				printf("ERROR: Couldn't find file '%s'.\n", argv[iArg]);
				result = -1;
				break;
			}

			do
			{
				char foundFile[BLPCONV_MAX_PATH];
				::sprintf(foundFile, "%s%s", filepath, aFindData.cFileName);
				//LOG("FindFirstFile found %s\n", foundFile);

				result = ProcessFile(foundFile, NULL);
				if (result != 0)
					break;
			}
			while (FindNextFile(hFindFile, &aFindData));

			FindClose(hFindFile);
#else 
			DIR *dp; 
			struct dirent *de; 

			dp = opendir( dirname( argv[iArg] )); 
			if( dp == NULL ) { 
			    printf("ERROR: Couldn't find file '%s' (%d).\n", argv[iArg], errno);
			    result = -1; 
			} else { 

			while ( (de = readdir(dp)) != NULL ) {
			    if( fnmatch( basename(argv[iArg]), de->d_name, FNM_PATHNAME ) == 0 ) { 
				result = ProcessFile(de->d_name, NULL);
				if( result != 0 ) { 
				    break; 
				}
			    }
			}
			closedir(dp); 

			}
#endif

			++iArg;
		}
		while ((result == 0) && (iArg < argc));
	}
	else
	{
		pszSourceFilename = argv[iArg++];
		if (!bMultiFileMode && iArg < argc)
		{
			pszDestFilename = argv[iArg];
		}

		result = ProcessFile(pszSourceFilename, pszDestFilename);
	}

Finish:
	if (bPauseAtEnd || (bPauseOnError && result != 0))
	{
		printf("Press any key to continue...\n");
		system("pause");
	}
	return result;
}

