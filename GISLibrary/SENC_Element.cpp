#include "stdafx.h"
#include "SENC_Element.h"

SENC_Element::SENC_Element()
{
}

SENC_Element::~SENC_Element()
{
	if (pColor) 
	{	
		delete pColor; 
		pColor = nullptr;	
	}
}
