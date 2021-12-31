#pragma once
#include "SENC_VectorPoint.h"

class SENC_Arc3Points
{
public:
	SENC_Arc3Points();
	~SENC_Arc3Points();

public:
	SENC_VectorPoint point;
	SENC_VectorPoint medianPoint;
	SENC_VectorPoint endPoint;
};