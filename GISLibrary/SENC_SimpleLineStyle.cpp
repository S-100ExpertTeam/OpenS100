#include "stdafx.h"
#include "SENC_SimpleLineStyle.h"

SENC_SimpleLineStyle::SENC_SimpleLineStyle()
{
	pen = NULL;
	dash = NULL;

}

SENC_SimpleLineStyle::~SENC_SimpleLineStyle()
{
	if (pen) delete pen;
	if (dash) delete dash;
}