#pragma once
#include "Geometry.h"

class GeoPointZ;

class GeoMultiPointZ :
	public Geometry
{
public:
	GeoMultiPointZ();
	GeoMultiPointZ(const GeoMultiPointZ& other);
	GeoMultiPointZ(int size);
	GeoMultiPointZ(double _x, double _y, double _z);
	GeoMultiPointZ(int size, double* _x, double* _y, double* _z);
	virtual ~GeoMultiPointZ(void);

public:
	int m_numPoints = 0;
	GeoPointZ* m_pPoints = nullptr;

public:
	virtual GeoMultiPointZ operator=(const GeoMultiPointZ& other);

public:
	void SetPoint(int index, double _x, double _y, double _z);
	void DrawGeometry(HDC &hDC, Scaler *scaler, double offset = 0);
};