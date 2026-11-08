#pragma once
#include "BinaryWriter.h"

class Directory
{
public:
	Directory();
	Directory(const Directory& other);
	Directory(char fieldTag[5], int fieldLength, int fieldPosition);
	~Directory();

public:
	char fieldTag[5] = { 0, };
	int fieldLength = 0;
	int fieldPosition = 0;

public:
	virtual Directory operator=(const Directory& other);

public:
	bool WriteDirectory(s100::io::BinaryWriter* file, int sizeOfFieldLengthField, int sizeOfFieldPositionField);
};