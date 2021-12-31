#pragma once
#include "AbstractLineStyle.h"

#include <string>

namespace LineStylesPackage
{
	class LineStyleReference : public AbstractLineStyle
	{
	public:
		LineStyleReference();
		virtual ~LineStyleReference();
	public:
		std::wstring reference;
	};
}