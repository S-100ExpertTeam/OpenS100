#pragma once

#include "SGeometry.h"
#include "SCurveHasOrient.h"

#include "../GeoMetryLibrary/GeoPoint.h"

class SCompositeCurve : public SGeometry
{
public:
	SCompositeCurve();
	virtual ~SCompositeCurve();

public:
	std::list<SCurveHasOrient*> m_listCurveLink;

public:
	void SetMBR();

	void CreateD2Geometry(ID2D1Factory1* factory);

	// Type : WKBMultiLineString
	bool ImportFromWkb(unsigned char* value, int size);
	bool ExportToWkb(unsigned char** value, int* size);
	bool ExportToWkbMultiLineString(unsigned char** value, int* size);
	
	// WKBLineString
	int WkbSize();

	// WKBMultiLineString
	int WkbSizeMultiLineString();

	void AddCurve(SCurveHasOrient* curve);

	void Release();

	int GetPointCount();

	GeoPoint GetXY(int index);
	void SetXY(int index, double x, double y);
};