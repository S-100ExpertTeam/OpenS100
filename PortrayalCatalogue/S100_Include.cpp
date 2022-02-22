#include "stdafx.h"
#include "S100_Include.h"

S100_Include::S100_Include()
{

}

S100_Include::~S100_Include()
{

}

void S100_Include::SetHerf(std::wstring& value)
{
	href = value;
}

std::wstring S100_Include::GetHerf()
{
	return href;
}