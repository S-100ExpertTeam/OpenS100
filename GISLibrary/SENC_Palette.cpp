#include "stdafx.h"
#include "SENC_Palette.h"


SENC_Palette::SENC_Palette()
{

}

SENC_Palette::~SENC_Palette()
{
	for (auto itor = items.begin(); itor != items.end(); itor++)
	{
		delete itor->second;
	}
}