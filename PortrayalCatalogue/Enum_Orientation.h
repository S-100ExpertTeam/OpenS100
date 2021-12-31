#pragma once
#include <string>

namespace PCD {
	enum class Orientation
	{
		none=0,
		forward=1,
		reverse=2
	};

	static const char* Orientation_String[] =
	{
		"none",
		"forward",
		"reverse"
	};

	Orientation StringToOrientation(std::string& value);
	Orientation StringToOrientation(std::wstring& value);

	std::string OrientationToString(Orientation value);
	std::wstring OrientationToWstring(Orientation value);
}