#pragma once
#include "SGeometry.h"

#include "..\\GeoMetryLibrary\\GeoPoint.h"

class SPoint : public SGeometry
{
public:
	SPoint();
	SPoint(double _x, double _y);
	virtual ~SPoint();

public:
	//GeoPoint m_point;	

	double x = 0;
	double y = 0;

	// It also has a CPoint arrangement to minimize memory allocation and recovery.
	POINT m_vPoint = {0, 0};

public:
	void SetPoint(double _x, double _y);
	void SetMBR();

	bool ImportFromWkb(unsigned char* value, int size);
	bool ExportToWkb(unsigned char** value, int* size);
};