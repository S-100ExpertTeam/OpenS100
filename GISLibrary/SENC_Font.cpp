#include "stdafx.h"
#include "SENC_Font.h"
#include "SENC_Define.h"

SENC_Font::SENC_Font()
{
	serifs = true;
	weight = 1;
	slant = 0;
	proportion = PROPORTIONAL;
}

SENC_Font::~SENC_Font()
{

}

void SENC_Font::setUpright()
{
	slant = 0;
}

bool SENC_Font::isUpright()
{
	return slant == 0;
}

void SENC_Font::setItalics()
{
	slant = 1;
}

bool SENC_Font::isItalics()
{
	return slant == 1;
}

DWRITE_FONT_STYLE SENC_Font::getFontStyle()
{
	if (slant == 1)
	{
		return DWRITE_FONT_STYLE_ITALIC;
	}

	return DWRITE_FONT_STYLE_NORMAL;
}