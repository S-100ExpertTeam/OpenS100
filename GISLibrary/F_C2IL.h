#pragma once
#include "Field.h"

struct C2IL;
class F_C2IL : Field
{
public:
	F_C2IL();
	virtual ~F_C2IL();

public:
	/*
	* Attribute Array
	*/
	std::list<C2IL*> m_arr;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file);

	int GetFieldLength();
};