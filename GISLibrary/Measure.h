#pragma once

#include "../FeatureCatalog/UnitOfMeasure.h"

class Measure
{
public:
	Measure();
	virtual ~Measure();

public:
	double value = 0;
	UnitOfMeasure unitOfMeasure;
};