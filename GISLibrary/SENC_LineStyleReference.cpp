#include "stdafx.h"
#include "SENC_LineStyleReference.h"
#include "SCurve.h"

#include "../PortrayalCatalogue/S100_LineStyleReference.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include "../GeoMetryLibrary/Scaler.h"

SENC_LineStyleReference::SENC_LineStyleReference()
{
	type = 2;
}

SENC_LineStyleReference::~SENC_LineStyleReference()
{

}

void SENC_LineStyleReference::GetStyleFromS100(S100_LineStyleReference* lineStyle)
{
	SetReference(lineStyle->GetReferenceW());
}

void SENC_LineStyleReference::SetReference(const std::wstring& value)
{
	reference = value;
}

std::wstring SENC_LineStyleReference::GetReference()
{
	return reference;
}
