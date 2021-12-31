#pragma once

#include <string>

namespace S100_SVG_D2D1_DLL 
{
	class S100_LineSymbol
	{
	public:
		S100_LineSymbol() {}
		virtual ~S100_LineSymbol() {}

	public:
		std::wstring reference;
		float position;
	};
}