#include "stdafx.h"
#include "LineSymbol.h"

namespace LineStylesPackage
{
	LineSymbol::LineSymbol()
	{
		capStyle = CapStyle::eCapStyle_butt;
		joinStyle = JoinStyle::eJoinStyle_miter;
		position = 0;
	}

	LineSymbol::~LineSymbol()
	{

	}
}
