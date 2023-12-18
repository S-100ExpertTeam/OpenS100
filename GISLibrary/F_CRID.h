#pragma once
#include "Field.h"
#include "RecordName.h"

class F_CRID : public Field
{
public:
	F_CRID();
	F_CRID(const F_CRID& other);
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

	virtual F_CRID* Clone() const;
};