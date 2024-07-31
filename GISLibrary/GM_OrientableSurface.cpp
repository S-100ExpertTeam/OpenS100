#include "stdafx.h"
#include "GM_OrientableSurface.h"

namespace GM
{
	OrientableSurface::OrientableSurface()
	{

	}

	OrientableSurface::~OrientableSurface()
	{

	}

	void OrientableSurface::setOrientation(bool value)
	{
		orientation = value;
	}

	bool OrientableSurface::getOrientation()
	{
		return orientation;
	}

	bool OrientableSurface::isForward()
	{
		return getOrientation();
	}
}