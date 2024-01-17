#pragma once

#include <vector>

class Field
{
public:
	Field();
	Field(const Field& other);
	virtual ~Field();

public:
	virtual void ReadField(BYTE*& buf) = 0;
	virtual bool WriteField(CFile* file) = 0;
	virtual int GetFieldLength() = 0;
};