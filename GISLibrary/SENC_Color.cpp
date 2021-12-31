#include "stdafx.h"
#include "SENC_Color.h"

#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"

SENC_Color::SENC_Color()
{
	RGBColor = 0;
	transparency = 0;
}

SENC_Color::~SENC_Color()
{

}

void SENC_Color::SetRGBColor(int value)
{
	RGBColor = value;
}

int SENC_Color::GetRGBColor()
{
	return RGBColor;
}

void SENC_Color::SetToken(std::wstring& value)
{
	token = value;
}

std::wstring SENC_Color::GetToken()
{
	return token;
}

void SENC_Color::SetTransparency(float value)
{
	transparency = value;
}

int SENC_Color::GetTransparency()
{
	return (int)transparency;
}

void SENC_Color::ChangePallete(PortrayalCatalogue *pc)
{
	auto colorProfile = pc->GetColorProfile();
	if (colorProfile)
	{
		auto paletteName = pc->GetCurrentPaletteName();
		auto rgb = colorProfile->GetRGBRef(paletteName, GetToken());
		SetRGBColor(rgb);
	}
}