#pragma once
#include "geometry.h"

class GeoPoint;
class GeoPolyline : public Geometry
{
public:
	GeoPolyline();
	GeoPolyline(const GeoPolyline& other);
	virtual ~GeoPolyline();

public:
	int NumParts = 0;
	int	NumPoints = 0;
	int* Parts = nullptr;
	GeoPoint* Points = nullptr;

public:
	virtual GeoPolyline operator=(const GeoPolyline& other);

public:
};