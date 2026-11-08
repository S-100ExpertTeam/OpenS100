#pragma once
#include <string>

class PortrayalCatalogue;
class SENC_AreaFillBase
{
public:
	SENC_AreaFillBase();
	virtual ~SENC_AreaFillBase();

public:	
	/*
	* 1 : ColorFill
	* 2 : AreaFillReference
	* 3 : PixmapFill
	* 4 : SymbolFill
	* 5 : HatchFill
	*/
	int type;
	std::wstring fileStem; 

public:
	void SetType(int value);
	int GetType();
	void SetFileStem(std::wstring& value);
	std::wstring GetFileStem();

public:
	virtual void ChangePallete(PortrayalCatalogue *pc) {};
};