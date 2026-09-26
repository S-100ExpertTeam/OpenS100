#include "stdafx.h"
#include "S100_WithParam.h"

S100_WithParam::S100_WithParam()
{

}

S100_WithParam::~S100_WithParam()
{

}

void S100_WithParam::SetName(const std::string& value)   { name = value; }
void S100_WithParam::SetName(const std::wstring& value)  { name = toUtf8(value); }
void S100_WithParam::SetSelect(const std::string& value)  { select = value; }
void S100_WithParam::SetSelect(const std::wstring& value) { select = toUtf8(value); }
void S100_WithParam::SetValue(const std::string& _value)  { value = _value; }
void S100_WithParam::SetValue(const std::wstring& _value) { value = toUtf8(_value); }

std::string  S100_WithParam::GetName()    { return name; }
std::wstring S100_WithParam::GetNameW()   { return toWide(name); }
std::string  S100_WithParam::GetSelect()  { return select; }
std::wstring S100_WithParam::GetSelectW() { return toWide(select); }
std::string  S100_WithParam::GetValue()   { return value; }
std::wstring S100_WithParam::GetValueW()  { return toWide(value); }
