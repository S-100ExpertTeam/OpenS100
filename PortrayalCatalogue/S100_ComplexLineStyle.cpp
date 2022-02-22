#include "stdafx.h"
#include "S100_ComplexLineStyle.h"

S100_ComplexLineStyle::S100_ComplexLineStyle()
{

}

S100_ComplexLineStyle::~S100_ComplexLineStyle()
{

}

void S100_ComplexLineStyle::SetSymbolReference(std::wstring& value)
{
	symbolReference = value;
}

std::wstring S100_ComplexLineStyle::GetSymbolReference()
{
	return symbolReference;
}

void S100_ComplexLineStyle::SetScaleFactor(std::wstring& value)
{
	scaleFactor = value;
}

std::wstring S100_ComplexLineStyle::GetScaleFactor()
{
	return scaleFactor;
}