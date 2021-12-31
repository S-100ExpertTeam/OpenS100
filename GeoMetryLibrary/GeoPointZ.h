#pragma once
#include "GeoPoint.h"

class GeoPointZ : public GeoPoint
{
public:
	GeoPointZ();
	GeoPointZ(double _x, double _y, double _z);
	virtual ~GeoPointZ();

public:
	double z = 0;

public:
	void SetPoint(double _x, double _y, double _z);
	void DrawGeometry(HDC &hDC, Scaler *scaler, double offset = 0);
	double GetZ();
};