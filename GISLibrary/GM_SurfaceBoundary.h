#pragma once

#include "GM_Ring.h"

#include <vector>

namespace S100Geometry
{
	class SurfaceBoundary
	{
	public:
		Ring exterior;
		std::vector<Ring> interior;
	};
}