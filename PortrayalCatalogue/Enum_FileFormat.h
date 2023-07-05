#pragma once

#include <string>

namespace Portrayal
{
	enum class FileFormat
	{
		none = 0,
		XML = 1,
		SVG = 2,
		XSLT = 3,
		TTF = 4,
		LUA = 5
	};

	static const char* FileFormat_String[] =
	{
		"none",
		"XML",
		"SVG",
		"XSLT",
		"TTF",
		"LUA"
	};

	FileFormat StringToFileFormat(std::string& value);
	FileFormat StringToFileFormat(std::wstring& value);

	std::string FileFormatToString(FileFormat value);
	std::wstring FileFormatToWString(FileFormat value);
}