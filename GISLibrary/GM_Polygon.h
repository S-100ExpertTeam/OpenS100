#pragma once

#include "GM_SurfacePatch.h"
#include "GM_SurfaceBoundary.h"

namespace S100Geometry
{
	class Polygon : public SurfacePatch
	{
	public:
		SurfaceBoundary boundary;
	};
}