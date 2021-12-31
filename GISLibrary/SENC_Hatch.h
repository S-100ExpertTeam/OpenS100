#pragma once
#include "SENC_LineStyleBase.h"
#include "SENC_VectorPoint.h"

class SENC_Hatch
{
public:
	SENC_Hatch();
	~SENC_Hatch();

public:
	SENC_LineStyleBase* lineStyle;
	SENC_VectorPoint direction;
	double distance; 
};