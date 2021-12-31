#pragma once
#include <string>

namespace S100_SVG_D2D1_DLL 
{
	class S100_LineColor 
	{
	public:
		S100_LineColor() {}
		virtual ~S100_LineColor() {}

	public:
		std::wstring name;
		std::wstring token;			
		std::wstring description;
		int r;
		int g;
		int b;
	};
}