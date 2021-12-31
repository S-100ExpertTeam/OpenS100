#pragma once
#include <string>

namespace S100_SVG_D2D1_DLL
{
	class S100_Text
	{
	public:
		S100_Text() {}
		virtual ~S100_Text() {}

	public:
		std::wstring text;
		float fontSize;
		POINTF point;
	};
}