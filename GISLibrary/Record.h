#pragma once

#include "Leader.h"
#include "Directory.h"

#include <list>

class DRDirectoryInfo;

class Record
{
public:
	Record();
	virtual ~Record();

	Leader leader;
	std::vector<Directory> directory;

public:
	virtual BOOL ReadRecord(DRDirectoryInfo* dir, BYTE*& buf) {	return TRUE; };
	virtual bool WriteRecord(CFile* file) { return true; };

	bool WriteDirectory(CFile* file);
	void SetLeader(int totalFieldSize, bool adjustEntryMap = true);
	int DirectoryLength(int sizeOfFieldLengthField, int sizeOfFieldPositionField);
};