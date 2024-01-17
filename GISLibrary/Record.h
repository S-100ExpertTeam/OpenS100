#pragma once

#include "Leader.h"
#include "Directory.h"
#include "RecordName.h"

#include <list>

class DRDirectoryInfo;

class Record
{
public:
	Record();
	Record(const Record& other);
	virtual ~Record();

	Leader leader;
	std::list<Directory> directory;

public:
	virtual Record operator=(const Record& other);

	virtual BOOL ReadRecord(DRDirectoryInfo* dir, BYTE*& buf) {	return TRUE; };
	virtual bool WriteRecord(CFile* file) { return true; };
	virtual RecordName GetRecordName() { return RecordName(); };

	bool WriteDirectory(CFile* file);
	void SetLeader(int totalFieldSize, bool adjustEntryMap = true);
	int DirectoryLength(int sizeOfFieldLengthField, int sizeOfFieldPositionField);
};