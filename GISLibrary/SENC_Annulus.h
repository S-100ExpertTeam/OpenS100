#pragma once
#include "SENC_VectorPoint.h"
#include "SENC_Sector.h"

class SENC_Annulus
{
public:
	SENC_Annulus();
	~SENC_Annulus();
	
public:
	SENC_VectorPoint point;
	double *innerRadius = nullptr;
	double outerRaius;
	SENC_Sector *sector = nullptr;
};