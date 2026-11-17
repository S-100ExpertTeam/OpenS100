#pragma once

#include "SENC_Text.h"
#include "SENC_Foreground.h"
#include "SENC_Font.h"
#include "DrawTypes.h"

class SENC_Element
{
public:
	SENC_Element();
	~SENC_Element();

public:
	SENC_Text text;
	int bodySize = 10;
	float verticalOffset;
	SENC_Foreground foreground;
	SENC_Font font;

	// For show text on screen
	GISLibrary::ColorF* pColor = nullptr;
};