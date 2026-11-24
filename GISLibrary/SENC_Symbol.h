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

	// `reference` in UTF-8, converted when first asked for and again when `reference` is another:
	// a renderer asks for it every time the symbol is drawn.
	const std::string& ReferenceUtf8();

private:
	std::string referenceUtf8;
	std::wstring referenceConverted;
	bool referenceRead = false;

	SENC_LinePlacement* linePlacement = nullptr;
	SENC_AreaPlacement* areaPlacement = nullptr;
};