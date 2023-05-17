#include "stdafx.h"
#include "GM_Surface.h"

namespace GM
{
	Surface::Surface()
	{

	}

	Surface::~Surface()
	{
	}

	Polygon Surface::GetPolygon()
	{
		return patch;
	}

	void Surface::SetExteriorRingID(std::string value)
	{
		patch.boundary.exterior.SetID(value);
	}

	void Surface::AddInteriorRingID(std::string value)
	{
		Ring ring;
		ring.SetID(value);
		patch.boundary.interior.push_back(ring);
	}
}