#include "stdafx.h"
#include "GM_MultiPoint.h"

namespace GM
{
	MultiPoint::MultiPoint()
	{

	}

	MultiPoint::~MultiPoint()
	{

	}

	void MultiPoint::Add(double x, double y, double z)
	{
		position.push_back(DirectPosition(x, y, z));
	}

	GeometryType MultiPoint::GetType()
	{
		return GeometryType::MultiPoint;
	}

	std::string MultiPoint::GeometricPrimitiveAsString()
	{
		return "MultiPoint";
	}
}