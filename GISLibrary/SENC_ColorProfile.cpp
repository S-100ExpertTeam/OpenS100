#include "stdafx.h"
#include "SENC_ColorProfile.h"

SENC_ColorProfile::SENC_ColorProfile()
{

}

SENC_ColorProfile::~SENC_ColorProfile()
{
	for (auto itor = colorMap.begin(); itor != colorMap.end(); itor++)
	{
		delete itor->second;
	}
	for (auto itor = paletteMap.begin(); itor != paletteMap.end(); itor++)
	{
		delete itor->second;
	}
}