#pragma once
#include "SGeometry.h"

#include "../GeoMetryLibrary/GeoPointZ.h"

class SMultiPoint : public SGeometry
{
public:
	SMultiPoint();
	SMultiPoint(const SMultiPoint& other);
	virtual ~SMultiPoint();

public:
	std::vector<GeoPointZ> m_pPoints;

public:
	virtual SMultiPoint operator=(const SMultiPoint& other);

public:
	SGeometryType GetType() override;

	int GetNumPoints();
	double GetX(int index);
	double GetY(int index);
	double GetZ(int index);
	void SetSize(int size);
	void SetMBR();

	void Add(double x, double y, double z);
	void Set(int index, double x, double y, double z);

	bool ImportFromWkb(unsigned char* value, int size);
	bool ExportToWkb(unsigned char** value, int* size);
};