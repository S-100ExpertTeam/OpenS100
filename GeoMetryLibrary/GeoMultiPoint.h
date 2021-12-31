#pragma once
#include "Geometry.h"

class GeoPoint;
class GeoMultiPoint :
	public Geometry
{
public:
	GeoMultiPoint();
	GeoMultiPoint(int size);
	GeoMultiPoint(double _x, double _y);
	GeoMultiPoint(int size, double* _x, double* _y);
	virtual ~GeoMultiPoint();

public:
	int m_numPoints = 0;
	GeoPoint* m_pPoints = nullptr;

public:
	void SetPoint(int index, double _x, double _y);
	void DrawGeometry(HDC &hDC, Scaler *scaler, double offset = 0);
};