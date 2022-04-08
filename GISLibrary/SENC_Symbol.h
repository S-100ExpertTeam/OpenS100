#pragma once
#include <string>
#include "SENC_LinePlacement.h"
#include "SENC_AreaPlacement.h"
#include "SENC_OverrideColor.h"
#include "SENC_VectorPoint.h"

#include "..\\S100_SVG_D2D1_DLL\SVG.h"

class SENC_Symbol
{
public:
	SENC_Symbol();
	virtual ~SENC_Symbol();

public:
	S100_SVG_D2D1_DLL::SVG* pSvg = nullptr;
	
	// element
	SENC_VectorPoint offset;
	SENC_Color overrideAll;
	SENC_OverrideColor overrideColor;
	SENC_LinePlacement* linePlacement = nullptr;
	SENC_AreaPlacement* areaPlacement = nullptr;

	// attribute
	std::wstring reference;
	double rotation = 0;
	int rotationCRS;
	float scaleFactor = 0;
};