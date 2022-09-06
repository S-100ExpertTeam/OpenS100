#pragma once

#include "Enum_RCNM.h"

#include <string>

class RecordName
{
public:
	RecordName();
	RecordName(int RCNM, int RCID);
	RecordName(enum GISLibrary::RCNM RCNM, int RCID);
	RecordName(long long key);
	
	virtual ~RecordName();

public:	
	/*
	* Referenced Record name
	* Format		: b11
	* Description	: Record name of the referenced record
	*/
	int RCNM = 0;
	/*
	* Referenced Record identifier
	* Format		: b14
	* Description	: Record identifier of the referenced record
	*/
	int RCID = 0;

public:
	__int64 GetName();
	int GetLength();
	std::wstring GetRCIDasWstring();
};