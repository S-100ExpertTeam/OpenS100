#include "stdafx.h"
#include "CompositeLineStyle.h"

namespace LineStylesPackage
{
	CompositeLineStyle::CompositeLineStyle()
	{
		subClassType = eCompositeLineStyle;
	}

	CompositeLineStyle::~CompositeLineStyle()
	{
		for (auto i = component.begin(); i != component.end(); i++)
		{
			delete *i;
		}
	}
}