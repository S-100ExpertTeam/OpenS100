#pragma once
#include "S100DrawingUnit.h"

class S100DrawingUnitAugmentedRay :
	public S100DrawingUnit
{
public:
	S100DrawingUnitAugmentedRay();
	virtual ~S100DrawingUnitAugmentedRay();

public:
	D2D1_COLOR_F _color = D2D1::ColorF(D2D1::ColorF::Black);
	FLOAT _width = 1;
	bool hasDash = false;
	D2D1_POINT_2F _geometryPoint;
	FLOAT _direction = 0;
	FLOAT _length = 0;
};