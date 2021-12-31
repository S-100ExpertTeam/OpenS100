#include "stdafx.h"
#include "SENC_LineStyleBase.h"

SENC_LineStyleBase::SENC_LineStyleBase()
{

}

SENC_LineStyleBase::~SENC_LineStyleBase()
{
	for (auto iter = geometryVector.begin(); iter != geometryVector.end(); iter++)
	{
		auto pGeometry = *iter;
		SafeRelease(& pGeometry);
	}
	geometryVector.clear();
}

void SENC_LineStyleBase::SetType(int value)
{
	type = value;
}

int SENC_LineStyleBase::GetType()
{
	return type;
}

bool SENC_LineStyleBase::IsLineStyle()
{
	return type == 1;
}

bool SENC_LineStyleBase::IsLineStyleReference()
{
	return type == 2;
}

bool SENC_LineStyleBase::IsCompositeLineStyle()
{
	return type == 3;
}