#pragma once

#include <string>

class SENC_Color
{
public:
	SENC_Color();
	~SENC_Color();

public:
	std::wstring token;
	float transparency;

public:
	void SetToken(const std::wstring& value);
	std::wstring GetToken();
	void SetTransparency(float value);
	int GetTransparency();
};