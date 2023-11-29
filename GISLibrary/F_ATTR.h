#pragma once

#include "Field.h"

#include <vector>

struct ATTR;
class F_ATTR : Field
{
public:
	F_ATTR();
	F_ATTR(const F_ATTR& other);
	virtual ~F_ATTR();

public:
	std::vector<ATTR*> m_arr;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file);
	int GetFieldLength();

	void Insert(ATTR* attr);
	void Insert(int natc, int atix, int paix, int atin, CString atvl);
	ATTR* getATTR(int index) const;
	int getCount() const;
};