#pragma once
#include "DrawingInstruction.h"
#include "LineStylesPackage.h"

class LineStylesPackage::AbstractLineStyle;

class LineInstruction :
	public DrawingInstruction
{
public:
	LineInstruction();
	virtual ~LineInstruction();

public:
	LineStylesPackage::AbstractLineStyle *pAbstractLineStyle;
	double width;
	CString colorName;
	bool hasDash;
	CString lineStyleReference;
};