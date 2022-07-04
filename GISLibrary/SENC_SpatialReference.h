#pragma once

#include <string>

class SENC_SpatialReference
{
public:
	SENC_SpatialReference();
	virtual ~SENC_SpatialReference();

public:
	int RCNM = 0;

	// RCID
	unsigned int reference = 0;

	bool bForward = true;

public:
	void SetRCNM(std::wstring& value);
	long long GetRecordName();
};