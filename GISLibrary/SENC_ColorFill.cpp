#include "stdafx.h"
#include "SENC_ColorFill.h"
#include "SENC_Color.h"

#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"

SENC_ColorFill::SENC_ColorFill()
{
	type = 1;
}

SENC_ColorFill::~SENC_ColorFill()
{
	if (color)
	{
		delete color;
	}
}

void SENC_ColorFill::ChangePallete(PortrayalCatalogue *pc)
{
	if (color)
	{
		color->ChangePallete(pc);
	}
}