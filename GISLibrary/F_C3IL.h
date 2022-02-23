#pragma once
#include "Field.h"

struct C3IL;
class F_C3IL : Field
{
public:
	F_C3IL();
	virtual ~F_C3IL();

public:
	/*
	* Vertical CRS Id
	* Format		: b11
	* Description	: Internal identifier of the Vertical CRS
	*/
	int m_vcid;
	/*
	* Attribute Array
	*/
	std::list<C3IL*> m_arr;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	int GetFieldLength();
};