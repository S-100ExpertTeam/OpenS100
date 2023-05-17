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
		virtual ~Polygon();

	public:
		SurfaceBoundary boundary;
	};
}