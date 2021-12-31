#pragma once
#include "SGeometry.h"

#include "..\\GeoMetryLibrary\\GeoPointZ.h"

class SMultiPoint : public SGeometry
{
public:
	SMultiPoint();
	virtual ~SMultiPoint();

public:
	int m_numPoints = 0;
	// For Sounding Depth
	int* pColor = nullptr;
	std::vector<GeoPointZ>* m_pPoints = nullptr;

public:
	int GetNumPoints();
	double GetX(int index);
	double GetY(int index);
};