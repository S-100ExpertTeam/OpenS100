#pragma once
#include "Geometry.h"

class GeoPoint : public Geometry
{
public:
	GeoPoint();
	GeoPoint(const GeoPoint& other);
	GeoPoint(double _x, double _y);
	virtual ~GeoPoint();

public:
	double x = 0;
	double y = 0;

public:
	virtual GeoPoint operator=(const GeoPoint& other);

public:
	void SetPoint(double _x, double _y);
	double GetX();
	double GetY();
	void DrawGeometry(HDC &hDC, Scaler *scaler, double offset = 0);
	bool operator == (GeoPoint& p);
};