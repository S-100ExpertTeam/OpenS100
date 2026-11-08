#include "stdafx.h"
#include "SENC_AreaFillBase.h"

SENC_AreaFillBase::SENC_AreaFillBase()
{

}

SENC_AreaFillBase::~SENC_AreaFillBase()
{

}

void SENC_AreaFillBase::SetType(int value)
{
	type = value;
}

int SENC_AreaFillBase::GetType()
{
	return type;
}

void SENC_AreaFillBase::SetFileStem(const std::wstring& value)
{
	fileStem = value;
}

std::wstring SENC_AreaFillBase::GetFileStem()
{
	return fileStem;
}