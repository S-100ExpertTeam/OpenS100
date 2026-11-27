#include "stdafx.h"
#include "GM_OrientableCurve.h"

#include "../LatLonUtility/cpp_util.h"

#include <regex>

namespace GM
{
	OrientableCurve::OrientableCurve()
	{

	}

	OrientableCurve::OrientableCurve(const OrientableCurve& item) : Primitive(item)
	{
		this->orientation = item.orientation;
	}

	OrientableCurve::~OrientableCurve()
	{
		//delete this->baseCurve;
		//this->baseCurve = nullptr;
	}

	GeometryType OrientableCurve::GetType()
	{
		return GeometryType::OrientableCurve;
	}

	std::string OrientableCurve::GeometricPrimitiveAsString()
	{
		return "Curve";
	}

	void OrientableCurve::Read(pugi::xml_node& /*node*/)
	{
		
	}

	int OrientableCurve::getPointCount()
	{

		return 0;
	}
		
	double OrientableCurve::getX(int /*index*/)
	{

		return 0;
	}

	double OrientableCurve::getY(int /*index*/)
	{

		return 0;
	}

	bool OrientableCurve::isForward()
	{
		return orientation;
	}
}