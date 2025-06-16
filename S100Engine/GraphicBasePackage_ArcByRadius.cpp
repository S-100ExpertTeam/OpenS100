#include "stdafx.h"
#include "GraphicBasePackage_ArcByRadius.h"

namespace GraphicBasePackage
{
	ArcByRadius::ArcByRadius()
	{

	}

	ArcByRadius::~ArcByRadius()
	{
		if (_sector)
		{
			delete _sector;
			_sector = nullptr;
		}
	}
}