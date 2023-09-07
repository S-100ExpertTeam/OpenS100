#include "stdafx.h"
#include "GM_SurfaceBoundary.h"

namespace GM
{

	SurfaceBoundary::SurfaceBoundary()
	{

	}

	SurfaceBoundary::SurfaceBoundary(SurfaceBoundary& item)
	{
		this->exterior = item.exterior;
		this->interior = item.interior;
	}

	SurfaceBoundary::~SurfaceBoundary()
	{

	}

}