#pragma once

#include "SGeometry.h"
#include "SCurve.h"

#include "../GeoMetryLibrary/GeoPoint.h"

class SCompositeCurve : public SGeometry
{
public:
	SCompositeCurve();
	virtual ~SCompositeCurve();

private:
	std::list<SCurve*> m_listCurveLink;

public:
	SGeometryType GetType() override;

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

	void AddCurve(SCurve* curve);

	void Release();

	int GetPointCount();

	GeoPoint GetXY(int index);
	void SetXY(int index, double x, double y);

	double GetX() override;
	double GetY() override; 

	int GetCurveCount();
	SCurve* GetCurve(int index);

	std::list<SCurve*> GetCurveList();
};