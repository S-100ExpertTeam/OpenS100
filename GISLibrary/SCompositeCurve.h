#pragma once

#include "SAbstractCurve.h"
#include "SCurve.h"

#include "../GeoMetryLibrary/GeoPoint.h"

class SCompositeCurve : public SAbstractCurve
{
public:
	SCompositeCurve();
	virtual ~SCompositeCurve();

private:
	std::list<SAbstractCurve*> m_listCurveLink;

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

	void AddCurve(SAbstractCurve* curve);

	void Release();

	int GetPointCount();
	int getNumPoint() override;

	GeoPoint GetXY(int index);
	void Set(int index, double x, double y) override;

	double GetX() override;
	double GetY() override; 
	double GetX(int index) override;
	double GetY(int index) override;

	int GetCurveCount();
	SAbstractCurve* GetCurve(int index);

	std::list<SAbstractCurve*> GetCurveList();
};