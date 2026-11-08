#include "stdafx.h"
#include "S100_AreaFillBase.h"

S100_AreaFillBase::S100_AreaFillBase()
{

}

S100_AreaFillBase::~S100_AreaFillBase()
{
}

void S100_AreaFillBase::SetType(int value) { type = value; }
int  S100_AreaFillBase::GetType()          { return type; }

void S100_AreaFillBase::SetFileStem(const std::string& value)  { fileStem = value; }
void S100_AreaFillBase::SetFileStem(const std::wstring& value) { fileStem = toUtf8(value); }
std::string  S100_AreaFillBase::GetFileStem()  { return fileStem; }
std::wstring S100_AreaFillBase::GetFileStemW() { return toWide(fileStem); }
