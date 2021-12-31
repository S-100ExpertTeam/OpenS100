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
	GeoPoint m_point;	
	// It also has a CPoint arrangement to minimize memory allocation and recovery.
	POINT   m_vPoint;

public:
	void SetPoint(double _x, double _y);
};