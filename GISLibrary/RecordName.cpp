#include "stdafx.h"
#include "RecordName.h"

RecordName::RecordName(void)
{

}

RecordName::RecordName(int RCNM, int RCID)
{
	this->RCNM = RCNM;
	this->RCID = RCID;
}

RecordName::RecordName(enum GISLibrary::RCNM RCNM, int RCID)
{
	this->RCNM = (int)RCNM;
	this->RCID = RCID;
}

RecordName::RecordName(long long key)
{
	this->RCNM = key >> 32;
	this->RCID = (int)key;
}

RecordName::~RecordName(void)
{

}

__int64 RecordName::GetName()
{
	return ((__int64)RCNM) << 32 | RCID;
}

int RecordName::GetLength()
{
	return 5;
}

std::string RecordName::GetRCIDasString(std::string prefix)
{
	return prefix + std::to_string(RCID);
}

std::wstring RecordName::GetRCIDasWstring() 
{
	return std::to_wstring(RCID);
}