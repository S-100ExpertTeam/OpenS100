#include "stdafx.h"
#include "GM_SurfacePatch.h"
#include "GM_Polygon.h"
#include "GM_SurfaceBoundary.h"

namespace GM
{
	Polygon::Polygon()
	{

	}

	Polygon::Polygon(Polygon& item)
	{
		this->boundary = item.boundary;
	}

	Polygon::~Polygon()
	{

	}
}