#pragma once

#include <string>

class PortrayalCatalogue;
class SENC_Color
{
public:
	SENC_Color();
	~SENC_Color();

public:
	D2D1_COLOR_F RGBColor;
	std::wstring token;
	float transparency;

public:
	void SetRGBColor(D2D1_COLOR_F value);
	D2D1_COLOR_F GetRGBColor();
	void SetToken(std::wstring& value);
	std::wstring GetToken();
	void SetTransparency(float value);
	int GetTransparency();
	void ChangePallete(PortrayalCatalogue *pc);
};