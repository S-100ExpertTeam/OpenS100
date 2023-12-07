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
	SSurface(const SSurface& other);
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

public: // override from Geometry
	bool ImportFromWkb(unsigned char* value, int size) override;
	bool ExportToWkb(unsigned char** value, int* size) override;
	int WkbSize() override;

	void SetMBR() override;
	void CreateD2Geometry(ID2D1Factory1* factory) override;

	void Release() override;

	double GetX() override;
	double GetY() override;
	double GetX(int index) override; 
	double GetY(int index) override;

	int getNumPoint() const override;

public: // override from SGeometry
	SGeometryType GetType() override;

public:
	int GetNumPart();
	
	int getPart(int index); // if fail, return -1

	int getLastPointIndexOfExterior();

	int GetNumPointPerPart(int partIndex);
	void CalculateCenterPoint();
	
	ID2D1PathGeometry* GetD2Geometry();
	ID2D1PathGeometry* GetNewD2Geometry(ID2D1Factory1* factory, Scaler* scaler);

	void AddCurve(SAbstractCurve* curve);
	
	void Init();
	void Set(std::vector<POINT>& points, std::vector<int>& parts); // parts : start index of next part
	//void Set(std::vector<GeoPoint> points, std::vector<int> partSize);

	GeoPoint GetXY(int ringIndex, int pointIndex);
	void SetXY(int ringIndex, int pointIndex, double x, double y);

	int GetRingCount() const;
	SAbstractCurve* GetRing(int index) const;

	SCurve* GetCurve(int rcid);

	// get all curve components
	void GetCurveList(std::list<SCurve*>& list);

	void setSuppress(bool value);
};