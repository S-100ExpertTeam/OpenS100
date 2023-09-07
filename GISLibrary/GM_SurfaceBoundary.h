#pragma once

#include "GM_Ring.h"

#include <vector>

namespace GM
{
	class SurfaceBoundary
	{
	public:
		SurfaceBoundary();
		SurfaceBoundary(SurfaceBoundary& item);
		virtual ~SurfaceBoundary();

	public:
		Ring* exterior = new Ring();
		std::vector<Ring*> interior;
	};
}