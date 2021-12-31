#pragma once
#include "CSSStyle.h"

namespace S100_SVG_D2D1_DLL
{
	class CSSStrokeLinecap :public CSSStyle
	{
	public:
		CSSStrokeLinecap();
		virtual ~CSSStrokeLinecap();

	public:
		int type;
	};
}