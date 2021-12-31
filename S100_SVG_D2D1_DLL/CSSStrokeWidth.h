#pragma once
#include "CSSStyle.h"

namespace S100_SVG_D2D1_DLL
{
	class CSSStrokeWidth :public CSSStyle
	{
	public:
		CSSStrokeWidth();
		virtual ~CSSStrokeWidth();
	
	public:
		float width;
	};
}