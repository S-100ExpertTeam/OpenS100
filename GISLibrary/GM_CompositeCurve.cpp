#include "stdafx.h"
#include "GM_CompositeCurve.h"

namespace GM
{
	CompositeCurve::CompositeCurve()
	{

	}

	CompositeCurve::~CompositeCurve()
	{

	}

	GeometryType CompositeCurve::GetType()
	{
		return GeometryType::CompositeCurve;
	}

	void CompositeCurve::Add(std::string curveMemberID)
	{
		component.push_back(OrientableCurve(curveMemberID));
	}

	void CompositeCurve::Add(GM::OrientableCurve item)
	{
		component.push_back(item);
	}

	void CompositeCurve::Add(GM::Curve item)
	{
		component.push_back(item);
	}

	void CompositeCurve::Add(GM::CompositeCurve item)
	{
		component.push_back(item);
	}
}