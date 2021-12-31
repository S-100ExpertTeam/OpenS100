#include "stdafx.h"
#include "SENC_Element.h"

SENC_Element::SENC_Element()
{
	pFont = NULL;
	pColor = NULL;
}

SENC_Element::~SENC_Element()
{
	if (pFont){		delete pFont; pFont = NULL; 	}
	if (pColor) {	delete pColor; pColor = NULL;	}
}
