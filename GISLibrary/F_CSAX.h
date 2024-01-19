#pragma once

#include "Field.h"

#include <list>

struct CSAX;

class F_CSAX : public Field
{
public:
	F_CSAX();
	F_CSAX(const F_CSAX& other);
	virtual ~F_CSAX();

public:
	std::list<CSAX*> m_arr;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file);

	int GetFieldLength();

	void Insert(int axty, int axum);

	virtual F_CSAX* Clone() const;
};