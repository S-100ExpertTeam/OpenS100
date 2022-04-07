#pragma once

#include "..\\S100_SVG_D2D1_DLL\\SVG.h"

class SENC_LineSymbol
{
public:
	SENC_LineSymbol();
	~SENC_LineSymbol();

public:
	double position;

	std::wstring reference;
	double rotation = 0;
	double scaleFactor;
	int crsType;

	S100_SVG_D2D1_DLL::SVG* pSvg;
};
