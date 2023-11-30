#pragma once

#include "Field.h"
#include "RecordName.h"
#include "F_ATTR.h"

#include <list>

class F_INAS : public Field
{
public:
	F_INAS();
	F_INAS(const F_INAS& other);
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
	int m_niac = 0;
	/*
	* Numeric AssociationRole code
	* Format		: b12
	* Description	: Record name of the referenced record
	*/
	int m_narc = 0;
	/*
	* Information Association Update Instruction
	* Format		: b11m 
	* {1} - Insert
	* {2} - Delete
	* {3} - Modify
	*/
	int m_iuin = 0;

	/*
	* Attribute Array
	*/
	std::vector<ATTR*> m_arr;
public:
	static int GetSize();

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file);
	int GetFieldLength();

	std::vector<ATTR*> GetAllAttributes();

	int getATTRCount() const;
	ATTR* getATTR(int index) const;
	void addATTR(ATTR* value);
};

