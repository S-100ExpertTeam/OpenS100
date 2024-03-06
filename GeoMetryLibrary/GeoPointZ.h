#pragma once
#include "GeoPoint.h"

class GeoPointZ : public GeoPoint
{
public:
	GeoPointZ();
	GeoPointZ(const GeoPointZ& other);
	GeoPointZ(double _x, double _y, double _z);
	virtual ~GeoPointZ();

public:
	double z = 0;

public:
	virtual GeoPointZ operator=(const GeoPointZ& other);

public:
	void SetPoint(double _x, double _y, double _z);
	void DrawGeometry(HDC& hDC, Scaler* scaler, double offset = 0);
	double GetZ();

	bool ImportFromWkb(unsigned char* value, int size);
	bool ExportToWkb(unsigned char** value, int* size);

	// Y X
	std::string ToString() override;
};