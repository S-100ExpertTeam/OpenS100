#include "stdafx.h"
#include "Enum_FileType.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

namespace Portrayal
{
	FileType StringToFileType(std::string& value)
	{
		int maxindex = (int)FileType::Rule + 1;
		for (int i = 0; i < maxindex; i++)
		{
			if (0 == value.compare(FileType_String[i]))
			{
				return FileType(i);
			}
		}
		return FileType::none;
	}

	FileType StringToFileType(std::wstring& value)
	{
		auto str = pugi::as_utf8(value);
		return StringToFileType(str);

	}

	std::string FileTypeToString(FileType value)
	{
		int index = (int)value;
		if (index < (int)FileType::none ||
			index > (int)FileType::Rule)
		{
			return "";
		}
		return FileType_String[index];
	}
	std::wstring FileTypeToWString(FileType value)
	{
		auto str = FileTypeToString(value);
		return pugi::as_wide(str);
	}
}