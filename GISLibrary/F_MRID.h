#pragma once
#include "Field.h"
#include "RecordName.h"

class F_MRID : public Field
{
public:
	F_MRID();
	F_MRID(const F_MRID& other);
	F_MRID(RecordName recordName, int RVER = 1, int RUIN = 1);
	virtual ~F_MRID();

public:
	RecordName m_name;
	int m_rver;
	int m_ruin;

public:
	void ReadField(BYTE *&buf);
	bool WriteField(CFile* file);
	int GetFieldLength();

	virtual F_MRID* Clone() const;
};