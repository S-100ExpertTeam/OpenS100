#pragma once
#include "Field.h"
#include "RecordName.h"

// Feature Association field
struct FASC;
class F_FASC : Field
{

public:
	F_FASC();
	F_FASC(const F_FASC& other);
	virtual ~F_FASC();

public:
	// Referenced Record
	RecordName m_name;
	/*
	* Numeric Feature Association Code
	* Format		: b12
	* Description	: A valid code for the feature association as defined in the FACS field of the Dataset General Information Record
	*/
	int m_nfac = 0;
	/*
	* Numeric AssociationRole Code
	* Format		: b12
	* Description	: A valid code for the role as defined in the ARCS field of the Dataset General Information Record
	*/
	int m_narc = 0;
	/*
	* Feature Association Update Instruction
	* Format		: b11
	* Description	:	{1} - Insert
						{2} – Delete
						{3} - Modify
	*/
	int m_faui = 0;

	std::vector<ATTR*> m_arr;
public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file);
	int GetFieldLength();

	int getATTRCount() const;
	ATTR* getATTR(int index) const;
	void addATTR(ATTR* value);
};