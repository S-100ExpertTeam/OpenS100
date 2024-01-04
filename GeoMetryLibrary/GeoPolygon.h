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
	// It also has a CPoint arrangement to minimize memory allocation and recovery.
	POINT* CPoints = nullptr;

public:
	virtual GeoPolygon operator=(const GeoPolygon& other);

public:
	void DrawGeometry(HDC &hDC, Scaler *scaler, double offset = 0);
};