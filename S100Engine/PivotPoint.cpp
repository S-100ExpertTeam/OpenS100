#include "stdafx.h"
#include "PivotPoint.h"

namespace libS100Engine
{
	PivotPoint::PivotPoint()
	{
		initialize();
	}

	PivotPoint::~PivotPoint()
	{

	}

	void PivotPoint::initialize()
	{
		type = pivotPoint;
		cx = cy = radius = strokeWidth = rotation = 0;
	}
}