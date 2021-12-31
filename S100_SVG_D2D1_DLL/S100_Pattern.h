#pragma once
#include <string>

namespace S100_SVG_D2D1_DLL 
{
	class S100_Pattern
	{
	public:
		S100_Pattern() {}
		virtual ~S100_Pattern() {}

	public:
		std::wstring symName;
		ID2D1PathGeometry* geom;
	};
}
