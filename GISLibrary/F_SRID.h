#pragma once
#include "Field.h"
#include "RecordName.h"

class F_SRID : public Field
{
public:
	F_SRID();
	F_SRID(const F_SRID& other);
	F_SRID(RecordName recordName, int RVER = 1, int RUIN = 1);
	virtual ~F_SRID();

public:
	RecordName m_name;
	int m_rver;
	int m_ruin;

public:
	virtual F_SRID operator=(const F_SRID& other);

public:
	void ReadField(BYTE *&buf);
	bool WriteField(CFile* file);

	int GetFieldLength();
};