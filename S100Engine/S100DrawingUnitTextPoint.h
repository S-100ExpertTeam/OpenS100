#pragma once
#include "S100DrawingUnit.h"

class S100DrawingUnitTextPoint :
	public S100DrawingUnit
{
public:
	S100DrawingUnitTextPoint();
	virtual ~S100DrawingUnitTextPoint();

public:
	D2D1_POINT_2F point;
	CString text;
	FLOAT rotation;
	FLOAT verticalOffset = 0;
	FLOAT xOffset = 0;
	FLOAT yOffset = 0;
	DWRITE_TEXT_ALIGNMENT textAlignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
	DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_FAR;
};

