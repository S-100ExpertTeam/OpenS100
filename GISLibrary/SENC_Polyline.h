#pragma once
#include "SENC_VectorPoint.h"

#include <list>

class SENC_Polyline
{
public:
	SENC_Polyline();
	virtual ~SENC_Polyline();

public:
	std::list<SENC_VectorPoint> points;
};