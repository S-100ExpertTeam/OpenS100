#pragma once

#define DR_LENGTH			24
#define RECORD_LENGTH		5
#define FIELD_LENGTH		1
#define FIELD_POSITION		1

#include <iostream>
#include <fstream>

class DRReader
{
public:
	DRReader(void);
	virtual ~DRReader(void);

public:
	static const int size = 24;
	int m_recordLength;
	int m_fieldAreaLoc;
	int m_fieldLength;
	int m_fieldPosition;

public:
	bool ReadReader(BYTE*& buf);
};