#pragma once
#include "CSSStyle.h"

namespace S100_SVG_D2D1_DLL
{
	class CSSStrokeLinejoin : public CSSStyle
	{
	public:
		CSSStrokeLinejoin();
		virtual ~CSSStrokeLinejoin();
	public:
		int type;
	};
}