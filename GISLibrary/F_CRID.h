#pragma once
#include "Field.h"
#include "RecordName.h"

class F_CRID : Field
{
public:
	F_CRID();
	virtual ~F_CRID();

public:
	RecordName m_name;
	int m_rver;
	int m_ruin;

public:
	void ReadField(BYTE *&buf);
	int GetFieldLength();
};