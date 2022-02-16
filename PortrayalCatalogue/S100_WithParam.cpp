#include "stdafx.h"
#include "S100_WithParam.h"

S100_WithParam::S100_WithParam()
{

}

S100_WithParam::~S100_WithParam()
{

}

void S100_WithParam::SetName(std::wstring& value)
{
	name = value;
}

void S100_WithParam::SetSelect(std::wstring& value)
{
	select = value;
}

void S100_WithParam::SetValue(std::wstring& _value)
{
	value = _value;
}

std::wstring S100_WithParam::GetName() 
{
	return name;
}

std::wstring S100_WithParam::GetSelect() 
{
	return select;
}

std::wstring S100_WithParam::GetValue()
{
	return value;
}