#include "stdafx.h"
#include "SENC_Symbol.h"
#include "SENC_Define.h"

SENC_Symbol::SENC_Symbol()
{
	rotationCRS = PORTRAYAL_CRS;
}

SENC_Symbol::~SENC_Symbol()
{
	if (linePlacement) delete linePlacement;
	if (areaPlacement) delete areaPlacement;
}