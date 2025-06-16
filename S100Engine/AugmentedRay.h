#pragma once
#include "AugmentedLineOrArea.h"
#include "GraphicBasePackage.h"

class AugmentedRay :
	public AugmentedLineOrArea
{
public:
	AugmentedRay();
	virtual ~AugmentedRay();

public:
	GraphicBasePackage::CRSType* _rotationCRS = nullptr;
	double dDirection = 0;
	double dLength = 0;
};