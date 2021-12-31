#pragma once

#include <string>

namespace PCD 
{
	enum class InterpolationType
	{
		none = 0,
		linear = 1,
		loxodromic = 2,
		circularArc3Points = 3,
		geodesic = 4,
		circularArcCenterPointWithRadius = 5,
		elliptical = 6,
		conic = 7
	};

	static const char* InterpolationType_String[] = {
		"none",
		"linear",
		"loxodromic" ,
		"circularArc3Points",
		"geodesic" ,
		"circularArcCenterPointWithRadius" ,
		"elliptical" ,
		"conic"
	};

	InterpolationType StringInterpolationType(std::string& value);
	InterpolationType StringInterpolationType(std::wstring& value);

	std::string InterpolationTypeToString(InterpolationType value);
	std::wstring InterpolationTypeToWString(InterpolationType value);
}