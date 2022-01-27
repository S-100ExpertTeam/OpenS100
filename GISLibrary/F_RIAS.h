#pragma once
#include "Field.h"
#include "RecordName.h"

struct RIAS;
class F_RIAS : Field
{
public:
	F_RIAS();
	virtual ~F_RIAS();
	
public:
	std::list<RIAS*> m_arr;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	int GetFieldLength();
};