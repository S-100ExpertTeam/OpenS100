#pragma once
#include <string>

namespace S100_SVG_D2D1_DLL
{
	class S100_LinePen
	{
	public:
		S100_LinePen();
		virtual ~S100_LinePen();

	public:
		float width;
		std::wstring col;
	};
}