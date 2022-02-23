#pragma once
#include "Field.h"
#include "RecordName.h"
class F_CCID : Field
{
public:
	F_CCID();
	virtual ~F_CCID();

public:
	RecordName m_name;
	int m_rver;
	int m_ruin;

public:
	void ReadField(BYTE *&buf);
	int GetFieldLength();
};