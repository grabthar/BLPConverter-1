#include "Log.h"

#include <ctime>
#include <fstream>

FORMATID Log::GetFormatFromString(const char* string)
{
	for (int iType = 1; iType < FORMAT_COUNT; ++iType)
	{
		if (0 == ::_stricmp(string, FORMATIDNames[iType]))
		{
			return (FORMATID)iType;
		}
	}

	LOG("ERROR: \"%s\" not a valid format string.\n", string);
	return FORMAT_UNSPECIFIED;
}

void Log::ListFormats()
{
	//LOG("****************\n");
	//LOG("* File Formats *\n");
	//LOG("****************\n");
	//LOG("Format Name\tDescription\n");
	//LOG("________________________________________________\n");
	//for (int ii = 1; ii < FORMAT_COUNT; ++ii)
	//	LOG("%s \t%s\n", FORMATIDNames[ii], FORMATIDDescriptions[ii]);
	//
	//LOG("\n**************************\n");
	//LOG("* Conversion Rules Table *\n");
	//LOG("**************************\n");
	//LOG("Source Format\t  Target Format\n");
	//LOG("________________________________________________\n");
	//for (int ii = 1; ii < FORMAT_COUNT; ++ii)
	//	LOG("%s \t> %s\n", FORMATIDNames[ii], FORMATIDNames[MemImage::s_ruleTable[ii]]);
	//
	//LOG("\nThe Conversion Rules table shows the format given to the destination file\nwhen the source file has a given format.  ");
	//LOG("You can change a rule with the /U\noption, or you can force the destination file into a given format with /F.\n");
	//LOG("/U can be specified multiple times.  BLP->BLP and PNG->PNG is OK.\n");
	//LOG("\nExamples:\n");
	//LOG("  blpconverter /FPNG_RGB myfile.blp\n");
	//LOG("  blpconverter /UBLP_PAL_A0=PNG_RGB /UPNG_PAL=PNG_RGB myfile.blp\n\n");

	std::string Description;
	std::string SourceTarget;
	for (int i = 0; i < FORMAT_COUNT; i++)
	{
		Description.append(std::string(FORMATIDNames[i] + "		" + FORMATIDDescriptions[i] + "\n"));
		SourceTarget.append(std::string(FORMATIDNames[i] + "		" + FORMATIDNames[MemImage::s_ruleTable[i]] + "\n"));
	}

	std::cout << std::endl
		<< "****************" << std::endl
		<< "* File Formats *" << std::endl
		<< "****************" << std::endl
		<< "Format Name\tDescription" << std::endl
		<< "________________________________________________" << std::endl
		<< "The Conversion Rules table shows the format given to the destination file\nwhen the source file has a given format.  " << std::endl
		<< "You can change a rule with the /U\noption, or you can force the destination file into a given format with /F." << std::endl
		<< "/U can be specified multiple times.  BLP->BLP and PNG->PNG is OK." << std::endl
		<< "Examples:" << std::endl
		<< "  blpconverter /FPNG_RGB myfile.blp" << std::endl
		<< "  blpconverter /UBLP_PAL_A0=PNG_RGB /UPNG_PAL=PNG_RGB myfile.blp" << std::endl;
}

void Log::WriteUsage()
{
	std::cout << std::endl
		<< "BLPCONVERTER: Converts BLP files to PNGs and vice versa." << std::endl
		<< "Version 8.4 (C) 2011 Patrick Cyr (kobuster@yahoo.com)" << std::endl
		<< "This program is free software under the GNU General Public License." << std::endl
		<< std::endl
		<< "BLPCONVERTER [options] sourceFile [targetFile | sourceFile [...]]" << std::endl
		<< "sourceFile The file to convert." << std::endl
		<< "targetFile Optionally, the name of the converted file.  If omitted, target file is given the same name as sourceFile but with the opposite extension." << std::endl
		<< std::endl
		<< "/A(value)			Sets the Alpha threshold when converting from palettized, 8-bit BLPs to palettized PNGs.  Value is a number between 0 and 255. Source alpha values below the threshold are fully transparent, above are fully opaque.  Default is" << MemImage::s_byAlphaThreshold << std::endl
		<< "/C					Create mip test image.  Outputs an image which contains all of the generated mip levels." << std::endl
		<< "/E					Pause on Error.  (Handy for drag-and-drop use.)" << std::endl
		<< "/F(format)			Forces target Format.  Overrides all other settings, including targetFile extension." << std::endl
		<< "/H					Force WoW cHaracter texture format (palettized, no alpha) when making BLPs." << std::endl
		<< "/I					Info mode.  Only outputs file information.  This option automatically sets the /V and /M options." << std::endl
		<< "/L					Lists formats and conversion rules." << std::endl
		<< "/M					Multi-file mode.  In this mode, multiple files can be input after options.  It is not possible to specify custom output names for them in this mode." << std::endl
		<< "/N					No mips.  Disables creation of mip levels when saving BLPs." << std::endl
		<< "/P					Pause upon completion. (Handy for drag-and-drop use.)" << std::endl
		<< "/R					Force WoW clothing texture formats.  All created BLPs are palettized and all PNGs are RGB/RGBA." << std::endl
		<< "/U(format)=(format) Change conversion rUle.  See /L." << std::endl
		<< "/V					Verbose mode. Outputs additional information." << std::endl;
}

std::string Log::FormatDateTimeXX(const int TimeValue)
{
	if (TimeValue < 10)
	{
		return std::to_string(TimeValue).insert(0, "0");
	}
	return std::to_string(TimeValue);
}

void Log::WriteLog(const std::string& Info)
{
	if (Info.empty())
	{
		return;
	}
	std::ofstream LogFile;
	time_t Now = time(0);
	tm* LocalTime = localtime(&Now);

	std::string DisplayDate("[" + FormatDateTimeXX(LocalTime->tm_mday) + "/" + FormatDateTimeXX(LocalTime->tm_mon + 1) + "/" + std::to_string(LocalTime->tm_year + 1900)
		+ " (" + FormatDateTimeXX(LocalTime->tm_hour) + ":" + FormatDateTimeXX(LocalTime->tm_min) + ":" + FormatDateTimeXX(LocalTime->tm_sec) + ")]	");

	LogFile.open("Log", std::ios::app);
	LogFile << DisplayDate << Info << std::endl;
	LogFile.close();
}
