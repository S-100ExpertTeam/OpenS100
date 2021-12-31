#include "stdafx.h"
#include "DrawingPattern.h"

namespace S100_SVG_D2D1_DLL
{
	//* Drawing Pattern
	DrawingPattern::DrawingPattern()
	{
		type = 0;
	}

	DrawingPattern::~DrawingPattern()
	{

	}

	void DrawingPattern::SetType(int value)
	{
		type = value;
	}

	int DrawingPattern::GetType()
	{
		return type;
	}

	void DrawingPattern::SetClass(std::wstring& value)
	{
		className = value;
	}

	const std::wstring& DrawingPattern::GetClass()
	{
		return className;
	}

	void DrawingPattern::SetStyle(std::wstring& value)
	{
		style = value;
	}

	const std::wstring& DrawingPattern::GetStyle()
	{
		return style;
	}
}