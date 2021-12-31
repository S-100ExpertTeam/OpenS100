#pragma once
#include "AugmentedGeometry.h"

class Point;
class Symbol;
class AugmentedPoint :
	public AugmentedGeometry
{
public:
	AugmentedPoint();
	virtual ~AugmentedPoint();

public:
	Point *pPoint;
	Symbol *pSymbol;
};