#pragma once
#include "AugmentedGeometry.h"
#include "LineStylesPackage.h"

class LineStylesPackage::AbstractLineStyle;
class AugmentedLineOrArea :
	public AugmentedGeometry
{
public:
	AugmentedLineOrArea();
	virtual ~AugmentedLineOrArea();

public:
	LineStylesPackage::AbstractLineStyle *pLineStyle = nullptr;
};