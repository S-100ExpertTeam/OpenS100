#pragma once
#include "Field.h"

struct CUCO;
class F_CUCO : Field
{
public:
	F_CUCO();
	virtual ~F_CUCO();
	
public:
	std::list<CUCO*> m_arr;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	int GetFieldLength();
};