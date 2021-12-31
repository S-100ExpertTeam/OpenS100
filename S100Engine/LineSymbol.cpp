#include "stdafx.h"
#include "lineSymbol.h"

namespace LineStylesPackage
{
	LineSymbol::LineSymbol()
	{
		capStyle = eCapStyle_butt;
		joinStyle = eJoinStyle_miter;
		position = 0;
	}

	LineSymbol::~LineSymbol()
	{

	}
}
