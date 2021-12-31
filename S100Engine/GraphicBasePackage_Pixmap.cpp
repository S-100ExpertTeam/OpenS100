#include "stdafx.h"
#include "GraphicBasePackage_Pixmap.h"

namespace GraphicBasePackage
{
	Pixmap::Pixmap()
	{

	}

	Pixmap::~Pixmap()
	{
		if (overrideAll)
		{
			delete overrideAll;
			overrideAll = nullptr;
		}

		if (override.size() > 0)
		{
			for (auto i = override.begin(); i != override.end(); i++)
			{
				delete (*i);
				(*i) = nullptr;
			}

			override.clear();
		}
	}
}