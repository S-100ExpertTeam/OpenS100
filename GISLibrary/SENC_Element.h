#pragma once
#include "SENC_Text.h"
#include "SENC_Foreground.h"
#include "SENC_Font.h"

class CFont;
class SENC_Element
{
public:
	SENC_Element();
	~SENC_Element();

public:
	SENC_Text text;
	int bodySize;
	float verticalOffset;
	SENC_Foreground foreground;
	SENC_Font font;
	// For show text on screen
	COLORREF* pColor;
	CFont* pFont;
};