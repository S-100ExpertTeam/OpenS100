#include "stdafx.h"
#include "Enum_Orientation.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

namespace PCD 
{
	Orientation StringToOrientation(std::string& value)
	{
		int maxindex = (int)Orientation::reverse+1;
		for (int i = 0; i < maxindex; i++) 
		{
			if (0==value.compare(Orientation_String[i]))
			{
				return (Orientation)i;
			}
		}
		return Orientation::none;
	}

	Orientation StringToOrientation(std::wstring& value) 
	{
		std::string str = pugi::as_utf8(value);
		return StringToOrientation(str);
	}

	std::string OrientationToString(Orientation value) 
	{
		auto index = (int)value;
		if (index < (int)Orientation::none ||
			index >(int)Orientation::reverse)
		{
			return "";
		}
		return Orientation_String[index];
	}

	std::wstring OrientationToWstring(Orientation value)
	{
		auto str = OrientationToString(value);
		return pugi::as_wide(str);
	}
}