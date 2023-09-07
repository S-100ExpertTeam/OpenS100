#pragma once

#include "GM_SurfacePatch.h"
#include "GM_SurfaceBoundary.h"

namespace GM
{
	class Polygon : 
		public SurfacePatch,
		public PugixmlElement
	{
	public:
		Polygon();
		Polygon(Polygon& item);
		virtual ~Polygon();

	public:
		SurfaceBoundary boundary;
	};
}