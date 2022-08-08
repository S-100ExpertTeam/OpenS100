#pragma once
#include "Field.h"
#include "RecordName.h"

#include <list>
#include <unordered_map>

struct MASK;
class F_MASK : Field
{
public:
	F_MASK();
	virtual ~F_MASK();

public:
	std::list<MASK*> listMask;
	std::unordered_map<__int64, MASK*> m_arr;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file);
	int GetFieldLength();

	void AddMask(MASK* mask);
};