#pragma once
#include "Field.h"
#include "RecordName.h"

#include <unordered_map>

struct MASK;
class F_MASK : Field
{
public:
	F_MASK();
	virtual ~F_MASK();

public:
	std::unordered_map<__int64, MASK*> m_arr;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	BOOL Save(CFile *file);
	int GetFieldLength();
};