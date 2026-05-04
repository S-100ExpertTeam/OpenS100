#include "stdafx.h"
#include "Enum_FileType.h"

namespace Portrayal
{
	FileType StringToFileType(const std::string& value)
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

	FileType StringToFileType(const std::wstring& value)
	{
		auto str = toUtf8(value);
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
		return toWide(FileTypeToString(value));
	}
}