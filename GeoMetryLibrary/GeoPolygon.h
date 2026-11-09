#pragma once
#include "geometry.h"

class GeoPoint;
class GeoPolygon : public Geometry
{
public:
	GeoPolygon();
	GeoPolygon(const GeoPolygon& other);
	virtual ~GeoPolygon();

public:
	int NumParts = 0;
	int	NumPoints = 0;
	int* Parts = nullptr;
	GeoPoint* Points = nullptr;

public:
	virtual GeoPolygon operator=(const GeoPolygon& other);

public:
};