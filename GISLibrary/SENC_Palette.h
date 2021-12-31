#pragma once
#include "SENC_Item.h"

#include <unordered_map>
#include <string>

class SENC_Palette
{
public:
	SENC_Palette();
	virtual ~SENC_Palette();

public:
	std::wstring name;
	std::unordered_map<std::wstring, SENC_Item*> items;
};