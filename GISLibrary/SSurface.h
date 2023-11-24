#pragma once

#include "SGeometry.h"
#include "SAbstractCurve.h"

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
	int* m_pParts = nullptr; // start index of part

	GeoPoint* m_pPoints = nullptr;
	GeoPoint* m_centerPoint = nullptr;

	// AreaD2Geometry
	ID2D1PathGeometry* pGeometry = nullptr;

private:
	// Rings
	std::vector<SAbstractCurve*> curveList;

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

	void AddCurve(SAbstractCurve* curve);
	
	void Init();
	void Set(std::vector<POINT>& points, std::vector<int>& parts); // parts : start index of next part

	void Release();

	bool ImportFromWkb(unsigned char* value, int size);
	bool ExportToWkb(unsigned char** value, int* size);
	int WkbSize();

	void SetMBR();

	GeoPoint GetXY(int ringIndex, int pointIndex);
	void SetXY(int ringIndex, int pointIndex, double x, double y);

	double GetX() override;
	double GetY() override;

	int GetRingCount() const;
	SAbstractCurve* GetRing(int index) const;

	//std::vector<SAbstractCurve*> GetCurveList();

	SCurve* GetCurve(int rcid);

	// get all curve components
	void GetCurveList(std::list<SCurve*>& list);

	void setSuppress(bool value);
};