#include "stdafx.h"
#include "SENC_SpatialReference.h"

SENC_SpatialReference::SENC_SpatialReference()
{

}

SENC_SpatialReference::~SENC_SpatialReference()
{

}

void SENC_SpatialReference::SetRCNM(std::wstring& value)
{

	if (value.compare(L"Point") == 0)
	{
		RCNM = 110;
	}
	else if (value.compare(L"MultiPoint") == 0)
	{
		RCNM = 115;
	}
	else if (value.compare(L"Curve") == 0)
	{
		RCNM = 120;
	}
	else if (value.compare(L"CompositeCurve") == 0)
	{
		RCNM = 125;
	}
	else if (value.compare(L"Surface") == 0)
	{
		RCNM = 130;
	}
}