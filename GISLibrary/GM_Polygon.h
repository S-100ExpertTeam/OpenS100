#pragma once

#include "GM_SurfacePatch.h"
#include "GM_SurfaceBoundary.h"

namespace GM
{
	class Polygon : 
		public SurfacePatch
	{
	public:
		Polygon();
		Polygon(const Polygon& item);
		virtual ~Polygon();

	public:
		SurfaceBoundary boundary;
	};
}