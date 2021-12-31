#pragma once
#include "SENC_Polyline.h"
#include "SENC_Arc3Points.h"
#include "SENC_ArcByRadius.h"
#include "SENC_Annulus.h"

#include <vector>

class SENC_Path
{
public:
	SENC_Path();
	virtual ~SENC_Path();

public:
	std::vector<SENC_Polyline*> polylines;
	std::vector<SENC_Arc3Points*> arc3Pointses;
	std::vector<SENC_ArcByRadius*> arcByRadiuses;
	std::vector<SENC_Annulus*> annuluses;
};