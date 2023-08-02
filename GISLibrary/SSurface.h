#pragma once

#include "SGeometry.h"
#include "SCurveHasOrient.h"

class Scaler;
class GeoPoint;
class MBR;
class SCompositeCurve;

class SSurface : public SGeometry
{
public:
	SSurface();
	SSurface(MBR* mbr);
	SSurface(std::vector<POINT>& points, std::vector<int>& parts);
	virtual ~SSurface();
	
public:
	// Polygon structure in the shp file
	int m_numParts = 0;
	int	m_numPoints = 0;
	int* m_pParts = nullptr;

	GeoPoint* m_pPoints = nullptr;
	GeoPoint* m_centerPoint = nullptr;

	// AreaD2Geometry
	ID2D1PathGeometry* pGeometry = nullptr;

private:
	// All curves that make up the Surface
	std::list<SCurveHasOrient*> curveList;

public:
	SGeometryType GetType() override;

	int GetNumPart();
	int getNumPoint();
	int getPart(int index); // if fail, return -1

	int getLastPointIndexOfExterior();

	int GetNumPointPerPart(int partIndex);
	void CalculateCenterPoint();
	
	void CreateD2Geometry(ID2D1Factory1* factory);
	
	ID2D1PathGeometry* GetD2Geometry();
	ID2D1PathGeometry* GetNewD2Geometry(ID2D1Factory1* factory, Scaler* scaler);

	void AddCurve(SCurveHasOrient* curve);
	void AddCompositeCurve(SCompositeCurve* compositeCurve);
	
	void Init();
	void Set(std::vector<POINT>& points, std::vector<int>& parts);

	void Release();

	bool ImportFromWkb(unsigned char* value, int size);
	bool ExportToWkb(unsigned char** value, int* size);
	int WkbSize();

	void SetMBR();

	GeoPoint GetXY(int ringIndex, int pointIndex);
	void SetXY(int ringIndex, int pointIndex, double x, double y);

	double GetX() override;
	double GetY() override;

	int GetRingCount();
	SCurveHasOrient* GetRing(int index);

	std::list<SCurveHasOrient*> GetCurveList();
};