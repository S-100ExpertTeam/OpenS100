#include "stdafx.h"
#include "SENC_Annulus.h"

SENC_Annulus::SENC_Annulus()
{

}

SENC_Annulus::~SENC_Annulus()
{
	if (innerRadius != nullptr)
	{
		delete innerRadius;
		innerRadius = nullptr;
	}

	if (sector != nullptr)
	{
		delete sector;
		sector = nullptr;
	}
}