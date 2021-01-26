#pragma once

#include "MemImage.h"
#include <iostream>
#include <string>


class Log
{
	FORMATID GetFormatFromString(const char* string);
	void ListFormats();
	void WriteUsage();
	std::string FormatDateTimeXX(const int TimeValue);
	void WriteLog(const std::string& Info);
};

