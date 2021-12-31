#include "StdAfx.h"
#include "RecordName.h"

RecordName::RecordName(void)
{

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

std::wstring RecordName::GetRCIDasWstring() 
{
	return std::to_wstring(RCID);
}