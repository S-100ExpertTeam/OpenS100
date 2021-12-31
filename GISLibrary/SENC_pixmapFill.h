#pragma once
#include "SENC_PatternFill.h"
#include "SENC_Pixmap.h"

class SENC_PixmapFill :
	public SENC_PatternFill
{
public:
	SENC_PixmapFill();
	virtual ~SENC_PixmapFill();

public:
	SENC_Pixmap pixmap;
};