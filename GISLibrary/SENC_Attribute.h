#pragma once
#include <string>

class SENC_Attribute
{
public:
	SENC_Attribute();
	~SENC_Attribute();

public:
	int parentIndex;
	std::wstring camelCase;
	std::wstring value;
};