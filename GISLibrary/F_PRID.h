#pragma once
#include "Field.h"
#include "RecordName.h"

class F_PRID : public Field
{
public:
	F_PRID();
	F_PRID(const F_PRID& other);
	F_PRID(RecordName recordName, int RVER = 1, int RUIN = 1);
	virtual ~F_PRID();

public:
	RecordName m_name;
	int m_rver;
	int m_ruin;

public:
	void ReadField(BYTE *&buf);
	bool WriteField(CFile* file);
	int GetFieldLength();

	virtual F_PRID* Clone() const;
};