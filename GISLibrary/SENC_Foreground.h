#pragma once
#include <string>

class SENC_Foreground
{
public:
	SENC_Foreground();
	~SENC_Foreground();

public:
	std::wstring token = L"";
	double transparency = 0;
};