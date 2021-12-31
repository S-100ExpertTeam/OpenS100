#pragma once
#include "SENC_Color.h"
#include "SENC_Palette.h"

#include <string>
#include <unordered_map>

class SENC_ColorProfile
{
public:
	SENC_ColorProfile();
	~SENC_ColorProfile();

public:
	std::unordered_map<std::wstring, SENC_Color*>  colorMap;
	std::unordered_map<std::wstring, SENC_Palette*>  paletteMap;
};