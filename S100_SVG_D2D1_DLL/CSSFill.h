#pragma once
#include "CSSStyle.h"

namespace S100_SVG_D2D1_DLL
{
	class CSSFill :public CSSStyle
	{
	public:
		CSSFill();
		virtual ~CSSFill();

	public:
		bool bFill;
		COLORREF color;
	};
}