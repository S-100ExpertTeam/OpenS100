#include "stdafx.h"
#include "GM_Ring.h"

namespace GM
{
	Ring::Ring()
	{

	}

	Ring::Ring(const Ring& item) : CompositeCurve(item)
	{
	}

	Ring::Ring(GM::CompositeCurve& curve)
	{
		
	}

	Ring::Ring(GM::OrientableCurve& curve)
	{

	}

	Ring::Ring(GM::Curve& curve)
	{

	}

	Ring::~Ring()
	{

	}
}