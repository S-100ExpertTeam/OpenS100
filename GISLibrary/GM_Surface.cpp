#include "stdafx.h"
#include "GM_Surface.h"

namespace GM
{
	Surface::Surface()
	{

	}

	Surface::Surface(const Surface& item) : OrientableSurface(item)
	{
		this->patch = item.patch;
	}

	Surface::~Surface()
	{

	}

	Polygon& Surface::GetPolygon()
	{
		return patch;
	}

	void Surface::SetExteriorRingID(std::string value)
	{
		patch.boundary.exterior->SetID(value);
	}

	void Surface::SetExteriorRing(GM::OrientableCurve* item)
	{
		//patch.boundary.exterior = (GM::Ring*)item;
		delete patch.boundary.exterior;
		patch.boundary.exterior = new GM::Ring(*item);
	}

	void Surface::SetExteriorRing(GM::Curve* item)
	{
		//patch.boundary.exterior = (GM::Ring*)item;
		delete patch.boundary.exterior;
		patch.boundary.exterior = new GM::Ring(*item);
	}

	void Surface::SetExteriorRing(GM::CompositeCurve* item)
	{
		//patch.boundary.exterior = (GM::Ring*)item;
		delete patch.boundary.exterior;
		patch.boundary.exterior = new GM::Ring(*item);
	}

	void Surface::AddInteriorRingID(std::string value)
	{
		Ring* ring = new Ring();
		ring->SetID(value);
		patch.boundary.interior.push_back(ring);
	}

	void Surface::AddInteriorRing(GM::OrientableCurve* item)
	{
		//patch.boundary.interior.push_back((GM::Ring*)item);
		patch.boundary.interior.push_back(new GM::Ring(*item));
	}

	void Surface::AddInteriorRing(GM::Curve* item)
	{
		//patch.boundary.interior.push_back((GM::Ring*)item);
		patch.boundary.interior.push_back(new GM::Ring(*item));
	}

	void Surface::AddInteriorRing(GM::CompositeCurve* item)
	{
		//patch.boundary.interior.push_back((GM::Ring*)item);
		patch.boundary.interior.push_back(new GM::Ring(*item));
	}

	GeometryType Surface::GetType()
	{
		return GeometryType::Surface;
	}

	std::string Surface::GeometricPrimitiveAsString()
	{
		return "Surface";
	}

	GM::Ring* Surface::getExteriorRing()
	{
		return patch.boundary.exterior;
	}

	int Surface::getInteriorRingCount()
	{
		return (int)patch.boundary.interior.size();
	}

	GM::Ring* Surface::getInteriorRing(int n)
	{
		if (n < 0 || n >= (int)patch.boundary.interior.size())
		{
			return nullptr;
		}
		
		return patch.boundary.interior[n];
	}
}