#include "stdafx.h"
#include "AugmentedPoint.h"
#include "Point.h"
#include "Symbol.h"

AugmentedPoint::AugmentedPoint()
{
	pPoint = nullptr;
	pSymbol = nullptr;
}

AugmentedPoint::~AugmentedPoint()
{
	delete pPoint;
	delete pSymbol;
}