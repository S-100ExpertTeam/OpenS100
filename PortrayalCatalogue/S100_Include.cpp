#include "stdafx.h"
#include "S100_Include.h"

S100_Include::S100_Include()
{

}

S100_Include::~S100_Include()
{

}

void S100_Include::SetHerf(const std::string& value)  { href = value; }
void S100_Include::SetHerf(const std::wstring& value) { href = toUtf8(value); }
std::string  S100_Include::GetHerf()  { return href; }
std::wstring S100_Include::GetHerfW() { return toWide(href); }
