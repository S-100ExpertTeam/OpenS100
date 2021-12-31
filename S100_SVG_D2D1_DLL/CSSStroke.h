#pragma once

#include "CSSStyle.h"

namespace S100_SVG_D2D1_DLL 
{
	class CSSStroke : public CSSStyle
	{
	public:
		CSSStroke();
		virtual ~CSSStroke();
	
	public:
		COLORREF color;
	};
}