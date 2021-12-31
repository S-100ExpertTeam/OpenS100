#pragma once
#include <string>

class S100_AreaFillBase
{
public:
	S100_AreaFillBase();
	virtual ~S100_AreaFillBase();

private:
	/*
	* 1 : ColorFill
	* 2 : AreaFillReference
	* 3 : PixmapFill
	* 4 : SymbolFill
	* 5 : HatchFill
	*/
	int type;
	std::wstring fileTitle;

public:
	void SetType(int value);
	int GetType();
	void SetFileTitle(std::wstring& value);
	std::wstring GetFileTitle();
};