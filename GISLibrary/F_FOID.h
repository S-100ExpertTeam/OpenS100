#pragma once
#include "Field.h"
#include "ObjectName.h"

class F_FOID : Field
{
public:
	F_FOID();
	virtual ~F_FOID();

public:
	ObjectName m_objName;

public:
	void ReadField(BYTE *&buf);
	BOOL Save(CFile *file);
	int GetFieldLength();
};