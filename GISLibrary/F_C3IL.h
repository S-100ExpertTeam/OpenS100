#pragma once
#include "Field.h"

struct C3IL;
class F_C3IL : public Field
{
public:
	F_C3IL();
	F_C3IL(const F_C3IL& other);
	virtual ~F_C3IL();

public:
	/*
	* Vertical CRS Id
	* Format		: b11
	* Description	: Internal identifier of the Vertical CRS
	*/
	int m_vcid = 2;
	/*
	* Attribute Array
	*/
	std::list<C3IL*> m_arr;

public:
	virtual F_C3IL operator=(const F_C3IL & other);

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file);
	int GetFieldLength();

	void Insert(int xcoo, int ycoo, int zcoo);
};
