#pragma once

#include "CSSStyle.h"

namespace S100_SVG_D2D1_DLL 
{
	class CSSOpacity :public CSSStyle
	{
	public:
		CSSOpacity();
		virtual ~CSSOpacity();

	public:
		float opacity;
	};
}