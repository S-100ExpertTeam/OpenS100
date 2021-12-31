#pragma once
#include "Field.h"
#include "RecordName.h"
#include "F_ATTR.h"

class F_INAS : Field
{
public:
	F_INAS();
	virtual ~F_INAS();
	/*
	* Referenced Record name/identifier
	*/
public:
	RecordName m_name;
	/*
	* Numeric Information Association Code
	* Format		: b12
	* Description	: Record name of the referenced record
	*/
	int m_niac;
	/*
	* Numeric AssociationRole code
	* Format		: b12
	* Description	: Record name of the referenced record
	*/
	int m_narc;
	/*
	* Information Association Update Instruction
	* Format		: b11m 
	* {1} - Insert
	* {2} - Delete
	* {3} - Modify
	*/
	int m_iuin;

	/*
	* Attribute Array
	*/
	CList<ATTR*> m_arr;
public:
	static int GetSize();

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	BOOL Save(CFile *file);
	int GetFieldLength();
};

