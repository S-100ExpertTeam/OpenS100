#include "stdafx.h"
#include "GM_Surface.h"

namespace S100Geometry
{
	Surface::Surface()
	{

	}

	Surface::~Surface()
	{
		delete patch;
		patch = nullptr;
	}

	Polygon* Surface::GetPolygon()
	{
		return (Polygon*)patch;
	}
}