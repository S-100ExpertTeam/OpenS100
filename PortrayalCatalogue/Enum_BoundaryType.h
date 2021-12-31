#pragma once
#include <string>

namespace PCD {
	enum class BoundaryType
	{
		none=0,
		begin=1,
		end=2
	};

	static const char* BoundaryType_String[] =
	{
		"none",
		"begin",
		"end"
	};

	BoundaryType StringToBoundaryType(std::string& value);
	BoundaryType StringToBoundaryType(std::wstring& value);

	std::string BoundaryTypeToString(BoundaryType value);
	std::wstring BoundaryTypeToWString(BoundaryType value);
}