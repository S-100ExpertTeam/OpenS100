#pragma once
#include <string>
#include "SENC_Rotation.h"
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
	S100_SVG_D2D1_DLL::SVG* pSvg;
	
	// element
	SENC_VectorPoint offset;
	SENC_Color overrideAll;
	SENC_OverrideColor overrideColor;
	SENC_LinePlacement *linePlacement;
	SENC_AreaPlacement *areaPlacement;

	// attribute
	std::wstring reference;
	SENC_Rotation *rotation;
	int rotationCRS;
	float scaleFactor;
};