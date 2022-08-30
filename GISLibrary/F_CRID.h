#pragma once
#include "Field.h"
#include "RecordName.h"

class F_CRID : Field
{
public:
	F_CRID();
	F_CRID(RecordName recordName, int RVER = 1, int RUIN = 1);
	virtual ~F_CRID();

public:
	RecordName m_name;
	int m_rver;
	int m_ruin;

public:
	void ReadField(BYTE *&buf);
	bool WriteField(CFile* file);

	int GetFieldLength();
};