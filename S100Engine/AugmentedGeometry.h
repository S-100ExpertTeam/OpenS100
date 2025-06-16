#pragma once
#include "DrawingInstruction.h"
#include "GraphicBasePackage.h"
#include "Text.h"

class TextPackage::Text;
class CRSType;
class AugmentedGeometry :
	public DrawingInstruction
{
public:
	AugmentedGeometry();
	virtual ~AugmentedGeometry();

public:
	CRSType *pCRSType = nullptr;
	TextPackage::Text *pText = nullptr;
	GraphicBasePackage::CRSType _crs = GraphicBasePackage::CRSType::geographicCRS;
	TextPackage::Text* _text = nullptr;
};