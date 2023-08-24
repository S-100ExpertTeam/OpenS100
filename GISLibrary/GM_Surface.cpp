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

	void Surface::SetExteriorRing(GM::OrientableCurve item)
	{
		patch.boundary.exterior = item;
	}

	void Surface::SetExteriorRing(GM::Curve item)
	{
		patch.boundary.exterior = item;
	}

	void Surface::SetExteriorRing(GM::CompositeCurve item)
	{
		patch.boundary.exterior = item;
	}

	void Surface::AddInteriorRingID(std::string value)
	{
		Ring ring;
		ring.SetID(value);
		patch.boundary.interior.push_back(ring);
	}

	void Surface::AddInteriorRing(GM::OrientableCurve item)
	{
		patch.boundary.interior.push_back(item);
	}

	void Surface::AddInteriorRing(GM::Curve item)
	{
		patch.boundary.interior.push_back(item);
	}

	void Surface::AddInteriorRing(GM::CompositeCurve item)
	{
		patch.boundary.interior.push_back(item);
	}

	GeometryType Surface::GetType()
	{
		return GeometryType::Surface;
	}

	std::string Surface::GeometricPrimitiveAsString()
	{
		return "Curve";
	}
}