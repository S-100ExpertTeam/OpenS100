#pragma once
#include "BinaryWriter.h"

#include <vector>

class Field
{
public:
	Field();
	Field(const Field& other);
	virtual ~Field();

public:
	virtual void ReadField(BYTE*& buf) = 0;
	virtual bool WriteField(s100::io::BinaryWriter* file) = 0;
	virtual int GetFieldLength() = 0;
};