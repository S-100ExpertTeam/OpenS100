#include "stdafx.h"
#include "Enum_BoundaryType.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

namespace PCD
{
	BoundaryType StringToBoundaryType(std::string& value) 
	{
		int maxindex = (int)BoundaryType::end + 1;
		for (int i = 0; i < maxindex; i++)
		{
			if (0==value.compare(BoundaryType_String[i]))
			{
				return BoundaryType(i);
			}
		}
		return BoundaryType::none;
	}
	BoundaryType StringToBoundaryType(std::wstring& value) 
	{
		auto str = pugi::as_utf8(value);
		return StringToBoundaryType(str);

	}

	std::string BoundaryTypeToString(BoundaryType value)
	{
		int index = (int)value;
		if (index<(int)BoundaryType::none||
			index>(int)BoundaryType::end)
		{
			return "";
		}
		return BoundaryType_String[index];
	}
	std::wstring BoundaryTypeToWString(BoundaryType value) 
	{
		auto str = BoundaryTypeToString(value);
		return pugi::as_wide(str);
	}
}