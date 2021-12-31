#include "stdafx.h"
#include "SENC_Hatch.h"
#include "SENC_LineStyle.h"
#include "SENC_LineStyleReference.h"
#include "SENC_CompositeLineStyle.h"

SENC_Hatch::SENC_Hatch()
{

}

SENC_Hatch::~SENC_Hatch()
{
	switch (lineStyle->type)
	{
	case 1:
		delete ((SENC_LineStyle*)lineStyle);
		break;
	case 2:
		delete ((SENC_LineStyleReference*)lineStyle);
		break;
	case 3:
		delete ((SENC_CompositeLineStyle*)lineStyle);
		break;
	}
}