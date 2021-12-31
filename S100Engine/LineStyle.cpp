#include "stdafx.h"
#include "LineStyle.h"
#include "LineSymbol.h"

namespace LineStylesPackage
{
	LineStyle::LineStyle()
	{
		intervalLength = 0;
		offset = 0;
		subClassType = eLineStyle;
		pen = nullptr;
	}

	LineStyle::~LineStyle()
	{
		for (auto i = dash.begin(); i != dash.end(); i++)
		{
			delete *i;
		}

		for (auto i = symbol.begin(); i != symbol.end(); i++)
		{
			delete *i;
		}

		delete pen;
	}
}