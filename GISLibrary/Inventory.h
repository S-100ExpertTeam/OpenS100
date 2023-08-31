#pragma once

#include "DataCoverage.h"

class Inventory
{
public:
	Inventory();
	Inventory(Inventory& item);
	virtual ~Inventory();

public:
	SSurface* geoPolygon = nullptr;
	std::vector<int> scaleBands;

public:
	void setScaleBands(DataCoverage dataCoverage);
};