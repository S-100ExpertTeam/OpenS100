#pragma once
#include "SENC_CIE.h"
#include "SENC_SRGB.h"

#include <string>

class SENC_Item
{
public:
	SENC_Item();
	~SENC_Item();

public:
	std::wstring token;
	SENC_CIE cie;
	SENC_SRGB rgb;
};