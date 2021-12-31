#include "stdafx.h"
#include "SENC_TextPoint.h"
#include "SENC_Define.h"

SENC_TextPoint::SENC_TextPoint()
{
	verticalAlignment = START;
	horizontalAlignment = TOP;
}

SENC_TextPoint::~SENC_TextPoint()
{
	for (auto itor = elements.begin(); itor != elements.end(); itor++)
	{
		delete *itor;
	}
}