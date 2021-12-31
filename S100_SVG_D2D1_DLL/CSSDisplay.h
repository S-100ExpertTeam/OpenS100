#pragma once
#include "CSSStyle.h"

namespace S100_SVG_D2D1_DLL
{
	class CSSDisplay : public CSSStyle
	{
	public:
		CSSDisplay();
		virtual ~CSSDisplay();

	public:
		bool bDisplay;
	};
}
