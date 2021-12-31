#pragma once
#include <string>

namespace PCD
{
	enum class GeometricPrimitive
	{
		none = 0,
		point = 1,
		multiPoint = 2,
		curve = 3,
		surface = 4,
		coverage = 5,
		complex=6
	};

	static const char* GeometricPrimitive_String[] = 
	{
		"none",
		"point",
		"multiPoint",
		"curve",
		"surface",
		"coverage",
		"complex"
	};
	GeometricPrimitive StringToGeometricPrimitive(std::string& value);
	GeometricPrimitive StringToGeometricPrimitive(std::wstring& value);

	std::string GeometricPrimitiveToString(GeometricPrimitive value);
	std::wstring GeometricPrimitiveToWString(GeometricPrimitive value);
}
