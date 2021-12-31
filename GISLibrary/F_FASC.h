#pragma once
#include "Field.h"
#include "RecordName.h"

// Feature Association field
struct FASC;
class F_FASC : Field
{

public:
	F_FASC();
	virtual ~F_FASC();

public:
	// Referenced Record
	RecordName m_name;
	/*
	* Numeric Feature Association Code
	* Format		: b12
	* Description	: A valid code for the feature association as defined in the FACS field of the Dataset General Information Record
	*/
	int m_nfac;
	/*
	* Numeric AssociationRole Code
	* Format		: b12
	* Description	: A valid code for the role as defined in the ARCS field of the Dataset General Information Record
	*/
	int m_narc;
	/*
	* Feature Association Update Instruction
	* Format		: b11
	* Description	:	{1} - Insert
						{2} – Delete
						{3} - Modify
	*/
	int m_faui;
	std::list<FASC*> m_arr;
public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	BOOL Save(CFile *file);
	int GetFieldLength();
};