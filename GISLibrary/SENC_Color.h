#pragma once

#include "DrawTypes.h"

#include <string>

class PortrayalCatalogue;
class SENC_Color
{
public:
	SENC_Color();
	~SENC_Color();

public:
	GISLibrary::ColorF RGBColor;
	std::wstring token;
	float transparency;

public:
	void SetRGBColor(GISLibrary::ColorF value);
	GISLibrary::ColorF GetRGBColor();
	void SetToken(const std::wstring& value);
	std::wstring GetToken();
	void SetTransparency(float value);
	int GetTransparency();
	void ChangePallete(PortrayalCatalogue *pc);
};