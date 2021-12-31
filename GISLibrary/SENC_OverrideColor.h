#pragma once
#include "SENC_Color.h"

class SENC_OverrideColor
{
public:
	SENC_OverrideColor();
	virtual ~SENC_OverrideColor();

public:
	SENC_Color overrideColor;
	SENC_Color color;
};