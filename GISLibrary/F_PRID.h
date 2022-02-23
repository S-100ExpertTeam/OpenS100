#pragma once
#include "Field.h"
#include "RecordName.h"

class F_PRID : Field
{
public:
	F_PRID();
	virtual ~F_PRID();

public:
	RecordName m_name;
	int m_rver;
	int m_ruin;

public:
	void ReadField(BYTE *&buf);
	int GetFieldLength();
};