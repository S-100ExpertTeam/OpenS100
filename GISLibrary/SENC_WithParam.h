#pragma once
#include <string>

class SENC_WithParam
{
public:
	SENC_WithParam();
	virtual ~SENC_WithParam();

public:
	std::wstring name;
	std::wstring select;
	std::wstring value;
};