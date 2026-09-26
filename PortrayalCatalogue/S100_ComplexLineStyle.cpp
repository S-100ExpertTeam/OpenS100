#include "stdafx.h"
#include "S100_ComplexLineStyle.h"

S100_ComplexLineStyle::S100_ComplexLineStyle()
{

}

S100_ComplexLineStyle::~S100_ComplexLineStyle()
{

}

void S100_ComplexLineStyle::SetSymbolReference(const std::string& value)  { symbolReference = value; }
void S100_ComplexLineStyle::SetSymbolReference(const std::wstring& value) { symbolReference = toUtf8(value); }
std::string  S100_ComplexLineStyle::GetSymbolReference()  { return symbolReference; }
std::wstring S100_ComplexLineStyle::GetSymbolReferenceW() { return toWide(symbolReference); }

void S100_ComplexLineStyle::SetScaleFactor(const std::string& value)  { scaleFactor = value; }
void S100_ComplexLineStyle::SetScaleFactor(const std::wstring& value) { scaleFactor = toUtf8(value); }
std::string  S100_ComplexLineStyle::GetScaleFactor()  { return scaleFactor; }
std::wstring S100_ComplexLineStyle::GetScaleFactorW() { return toWide(scaleFactor); }

void S100_ComplexLineStyle::GetContents(pugi::xml_node& node)
{
}
