#pragma once

#include <vector>

#include "LineStylesPackageEnum.h"

namespace LineStylesPackage
{
	class LineStyle;
	class AbstractLineStyle
	{

	public:
		AbstractLineStyle();
		virtual ~AbstractLineStyle();

	public:
		SubClassTypeOfAbstractLineStyle subClassType;
		std::wstring name;

	public:
		int         GetLineStyleCount();
		LineStyle * GetLineStyle(int index);
		void        GetRect(D2D1_RECT_F *pRect);
	};
}