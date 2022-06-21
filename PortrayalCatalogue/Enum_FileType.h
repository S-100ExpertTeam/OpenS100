#pragma once

#include <string>

namespace Portrayal
{
	enum class FileType
	{
		none = 0,
		Font = 1,
		AreaFill = 2,
		LineStyle = 3,
		Symbol = 4,
		ColorProfile = 5,
		Pixmap = 6,
		Rule = 7,
		StyleSheet = 8,
		AlertCatalog = 9
	};

	static const char* FileType_String[] =
	{
		"none",
		"Font",
		"AreaFill",
		"LineStyle",
		"Symbol",
		"ColorProfile",
		"Pixmap",
		"Rule",
		"StyleSheet",
		"AlertCatalog"
	};

	FileType StringToFileType(std::string& value);
	FileType StringToFileType(std::wstring& value);

	std::string FileTypeToString(FileType value);
	std::wstring FileTypeToWString(FileType value);
}