#pragma once

#include <vector>

class Field
{
public:
	Field();
	virtual ~Field();

public:
	virtual void ReadField(BYTE *&buf) {};
	virtual bool WriteField(CFile* file) { return true; };
	virtual int GetFieldLength() { return 0; };
};