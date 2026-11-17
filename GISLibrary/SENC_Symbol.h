#pragma once

#include <string>

#include "SENC_LinePlacement.h"
#include "SENC_AreaPlacement.h"
#include "SENC_OverrideColor.h"
#include "SENC_VectorPoint.h"

class SENC_Symbol
{
public:
	SENC_Symbol();
	virtual ~SENC_Symbol();

public:

	// element
	SENC_VectorPoint offset;
	SENC_Color overrideAll;
	SENC_OverrideColor overrideColor;


	// attribute
	std::wstring reference;
	double rotation = 0;
	int rotationCRS;
	float scaleFactor = 0;

private:
	SENC_LinePlacement* linePlacement = nullptr;
	SENC_AreaPlacement* areaPlacement = nullptr;
};