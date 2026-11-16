#pragma once
#include "SGeometry.h"

#include "SGeometry.h"

class SPoint : public SGeometry
{
public:
	SPoint();
	SPoint(const SPoint& other);
	SPoint(double _x, double _y);
	virtual ~SPoint();

public:
	//GeoPoint m_point;	

	double x = 0;
	double y = 0;


public:
	virtual SPoint operator=(const SPoint& other);

public:
	SGeometryType GetType() override;

	void SetPoint(double _x, double _y);
	void SetMBR();

	bool ImportFromWkb(unsigned char* value, int size);
	bool ExportToWkb(unsigned char** value, int* size);

	bool operator==(const SPoint& point) const;

	double GetX() override;
	double GetY() override;

	// Y X
	std::string ToString() override;
};