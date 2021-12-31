#pragma once
#include <string>

namespace S100_SVG_D2D1_DLL
{
	class RealLine
	{
	public:
		RealLine() {}
		virtual ~RealLine() {}
	
	public:
		std::wstring lineName;
		D2D1_POINT_2F startPos;
		D2D1_POINT_2F endPos;
	};
}