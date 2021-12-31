#pragma once
#include <vector>

#include "AbstractLineStyle.h"

namespace LineStylesPackage
{
	class CompositeLineStyle : public AbstractLineStyle
	{
	public:
		CompositeLineStyle();
		virtual ~CompositeLineStyle();

	public:
		std::vector<AbstractLineStyle*> component;
	};
}