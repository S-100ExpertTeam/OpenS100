#include "stdafx.h"
#include "Enum_InterpolationType.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

namespace PCD 
{
	InterpolationType StringInterpolationType(std::string& value) 
	{
		int maxindex = (int)InterpolationType::conic + 1;
		for (int i =0; i<maxindex; i++) 
		{
			if (0==value.compare(InterpolationType_String[i]))
			{
				return InterpolationType(i);
			}
		}
		return InterpolationType::none;
	}
	InterpolationType StringInterpolationType(std::wstring& value) 
	{
		auto str = pugi::as_utf8(value);
		return StringInterpolationType(str);
	}
	std::string InterpolationTypeToString(InterpolationType value) 
	{
		int index = (int)value;
		if (index<(int)InterpolationType::none ||
			index>(int)InterpolationType::conic)
		{
			return "";
		}
		return InterpolationType_String[index];
	}
	std::wstring InterpolationTypeToWString(InterpolationType value) 
	{
		auto str = InterpolationTypeToString(value);
		return pugi::as_wide(str);
	}
}