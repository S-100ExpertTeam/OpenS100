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
		delete exterior;
		exterior = nullptr;

		for (auto i = interior.begin(); i != interior.end(); i++) {
			delete (*i);
			(*i) = nullptr;
		}

		interior.clear();
	}

}