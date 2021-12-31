#pragma once
#include "SENC_Color.h"
#include "SENC_OverrideColor.h"

class SENC_Pixmap
{
public:
	SENC_Pixmap();
	virtual ~SENC_Pixmap();

public:
	unsigned reference;
	SENC_Color overrideColor;
	SENC_OverrideColor overrideAll;
};