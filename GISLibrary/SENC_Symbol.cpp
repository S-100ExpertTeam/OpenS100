#include "stdafx.h"
#include "SENC_Symbol.h"
#include "SENC_Define.h"

SENC_Symbol::SENC_Symbol()
{
	rotationCRS = PORTRAYAL_CRS;
	scaleFactor = 0;

	rotation = NULL;
	linePlacement = NULL;
	areaPlacement = NULL;

	pSvg = NULL;
}

SENC_Symbol::~SENC_Symbol()
{
	if (rotation) delete rotation;
	if (linePlacement) delete linePlacement;
	if (areaPlacement) delete areaPlacement;
}