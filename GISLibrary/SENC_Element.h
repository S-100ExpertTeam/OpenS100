#pragma once

#include "SENC_Text.h"
#include "SENC_Foreground.h"
#include "SENC_Font.h"

class SENC_Element
{
public:
	SENC_Element();
	~SENC_Element();

public:
	SENC_Text text;
	int bodySize = 10;
	float verticalOffset = 0;
	SENC_Foreground foreground;
	SENC_Font font;

	// Whether the instruction gave this element a foreground colour (foreground.token). Text
	// without one is drawn in whatever colour was last used.
	bool hasForeground = false;
};