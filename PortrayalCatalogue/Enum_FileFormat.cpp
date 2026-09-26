#include "stdafx.h"
#include "Enum_FileFormat.h"

namespace Portrayal
{
	FileFormat StringToFileFormat(const std::string& value)
	{
		int maxindex = (int)FileFormat::LUA + 1;
		for (int i = 0; i < maxindex; i++)
		{
			if (0 == value.compare(FileFormat_String[i]))
			{
				return FileFormat(i);
			}
		}
		return FileFormat::none;
	}

	FileFormat StringToFileFormat(const std::wstring& value)
	{
		auto str = toUtf8(value);
		return StringToFileFormat(str);
	}

	std::string FileFormatToString(FileFormat value)
	{
		int index = (int)value;
		if (index<(int)FileFormat::none ||
			index>(int)FileFormat::LUA)
		{
			return "";
		}
		return FileFormat_String[index];
	}

	std::wstring FileFormatToWString(FileFormat value)
	{
		return toWide(FileFormatToString(value));
	}
}