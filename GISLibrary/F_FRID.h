#pragma once
#include "Field.h"
#include "RecordName.h"

class F_FRID : public Field
{
public:
	F_FRID();
	F_FRID(const F_FRID& other);
	F_FRID(RecordName recordName, int NFTC, int RVER = 1, int RUIN = 1);
	virtual ~F_FRID();

public:
	RecordName m_name;
	/*
	* Numeric Feature Type Code
	* Format		: b12
	* Description	: A valid feature type code as defined in the FTCS field of the Dataset General Information Record
	*/
	int m_nftc = 0;
	int m_rver = 0;
	int m_ruin = 0;

public:
	void ReadField(BYTE *&buf);
	bool WriteField(CFile* file);
	int GetFieldLength();

	virtual F_FRID* Clone() const;
};