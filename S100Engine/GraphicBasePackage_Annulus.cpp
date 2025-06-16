#include "stdafx.h"
#include "GraphicBasePackage_Annulus.h"

namespace GraphicBasePackage
{
	Annulus::Annulus()
	{

	}

	Annulus::~Annulus()
	{
		if (_innerRadius)
		{
			delete _innerRadius;
			_innerRadius = nullptr;
		}

		if (_sector)
		{
			delete _sector;
			_sector = nullptr;
		}
	}
}