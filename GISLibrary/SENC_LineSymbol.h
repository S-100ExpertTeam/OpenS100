#pragma once

class SENC_LineSymbol
{
public:
	SENC_LineSymbol();
	~SENC_LineSymbol();

public:
	double position = 0;

	std::wstring reference;
	double rotation = 0;
	double scaleFactor = 1;
	int crsType = 0;

};
