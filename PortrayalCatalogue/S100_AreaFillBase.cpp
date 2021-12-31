#include "stdafx.h"
#include "S100_AreaFillBase.h"

S100_AreaFillBase::S100_AreaFillBase()
{

}

S100_AreaFillBase::~S100_AreaFillBase()
{
}

void S100_AreaFillBase::SetType(int value) 
{
	type = value;
}

int S100_AreaFillBase::GetType()
{
	return type;
}

void S100_AreaFillBase::SetFileTitle(std::wstring& value)
{
	fileTitle = value;
}


std::wstring S100_AreaFillBase::GetFileTitle()
{
	return fileTitle;
}