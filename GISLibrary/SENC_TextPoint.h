#pragma once
#include "SENC_Element.h"
#include "SENC_VectorPoint.h"

#include <vector>

class SENC_TextPoint
{
public:
	SENC_TextPoint();
	virtual ~SENC_TextPoint();

public:
	int horizontalAlignment;
	int verticalAlignment;
	std::vector<SENC_Element*> elements;
	SENC_VectorPoint offset;
};