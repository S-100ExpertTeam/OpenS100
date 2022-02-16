#pragma once
#include <string>

class S100_ComplexLineStyle
{
public:
	S100_ComplexLineStyle();
	virtual ~S100_ComplexLineStyle();

private:
	std::wstring symbolReference;
	std::wstring scaleFactor;

public:
	void SetSymbolReference(std::wstring& value);
	std::wstring GetSymbolReference();

	void SetScaleFactor(std::wstring& value);
	std::wstring GetScaleFactor();
};