#pragma once
#include "SENC_VectorPoint.h"
#include "SENC_Sector.h"

class SENC_ArcByRadius
{
public:
	SENC_ArcByRadius();
	~SENC_ArcByRadius();

public:
	SENC_VectorPoint center;
	SENC_Sector *sector = nullptr;
	double radius;
};