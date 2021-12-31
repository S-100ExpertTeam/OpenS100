#pragma once
#include "SENC_Color.h"

class SENC_Pen
{
public:
	SENC_Pen();
	virtual ~SENC_Pen();

public:
	SENC_Color color;
	int width;
};