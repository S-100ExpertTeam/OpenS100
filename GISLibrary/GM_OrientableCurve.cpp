#include "stdafx.h"
#include "GM_OrientableCurve.h"

namespace GM
{
	OrientableCurve::OrientableCurve()
	{

	}

	OrientableCurve::OrientableCurve(std::string baseCurveID)
	{
		this->baseCurveID = baseCurveID;
	}
	
	OrientableCurve::~OrientableCurve()
	{

	}
}