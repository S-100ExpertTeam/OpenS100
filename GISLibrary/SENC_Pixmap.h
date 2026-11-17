#pragma once
#include "SENC_Color.h"
#include "SENC_OverrideColor.h"

class SENC_Pixmap
{
public:
	SENC_Pixmap();
	virtual ~SENC_Pixmap();

public:
	unsigned reference = 0;
	SENC_Color overrideColor;
	SENC_OverrideColor overrideAll;
};