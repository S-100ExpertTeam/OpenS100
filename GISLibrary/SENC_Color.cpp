#include "stdafx.h"
#include "SENC_Color.h"


SENC_Color::SENC_Color()
{
	//RGBColor = 0;
	transparency = 0;
}

SENC_Color::~SENC_Color()
{

}

void SENC_Color::SetRGBColor(GISLibrary::ColorF value)
{
	RGBColor = value;
}

GISLibrary::ColorF SENC_Color::GetRGBColor()
{
	return RGBColor;
}

void SENC_Color::SetToken(const std::wstring& value)
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
