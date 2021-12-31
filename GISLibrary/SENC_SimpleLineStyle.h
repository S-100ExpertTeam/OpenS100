#pragma once
#include "SENC_Pen.h"
#include "SENC_Dash.h"

class SENC_SimpleLineStyle
{
public:
	SENC_SimpleLineStyle();
	virtual ~SENC_SimpleLineStyle();

public:
	int capStyle;
	int joinStyle;
	float offset;
	SENC_Pen *pen;
	SENC_Dash *dash;
};