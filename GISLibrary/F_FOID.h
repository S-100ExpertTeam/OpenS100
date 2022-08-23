#pragma once
#include "Field.h"
#include "ObjectName.h"

class F_FOID : Field
{
public:
	F_FOID();
	F_FOID();
	virtual ~F_FOID();

public:
	ObjectName m_objName;

public:
	void ReadField(BYTE *&buf);
	bool WriteField(CFile* file);

	int GetFieldLength();
};