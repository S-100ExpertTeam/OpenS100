#pragma once
#include <string>

class SENC_Foreground
{
public:
	SENC_Foreground();
	~SENC_Foreground();

public:
	std::wstring token;
	int transparency;
};