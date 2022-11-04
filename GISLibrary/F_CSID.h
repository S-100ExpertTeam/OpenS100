#pragma once
#include "Field.h"
#include "RecordName.h"

class F_CSID : Field
{
public:
	F_CSID();
	virtual ~F_CSID();

public:
	RecordName m_name;
	int m_ncrc;

public:
	void ReadField(BYTE *&buf);
	bool WriteField(CFile* file);

	int GetFieldLength();
};