#include "stdafx.h"
#include "SENC_ArcByRadius.h"

SENC_ArcByRadius::SENC_ArcByRadius()
{

}

SENC_ArcByRadius::~SENC_ArcByRadius()
{
	if (sector != nullptr)
	{
		delete sector;
		sector = nullptr;
	}

}