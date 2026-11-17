#pragma once
#include "SENC_LineStyleBase.h"
#include "SENC_VectorPoint.h"

class SENC_Hatch
{
public:
	SENC_Hatch();
	~SENC_Hatch();

public:
	SENC_LineStyleBase* lineStyle = nullptr;
	SENC_VectorPoint direction;
	double distance = 0;
};