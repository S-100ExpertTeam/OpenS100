#pragma once

#include "GM_SurfacePatch.h"
#include "GM_SurfaceBoundary.h"

namespace GM
{
	class Polygon : public SurfacePatch
	{
	public:
		SurfaceBoundary boundary;
	};
}