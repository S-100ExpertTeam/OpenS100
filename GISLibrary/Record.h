#pragma once

#include "Leader.h"
#include "Directory.h"

#include <vector>

class DRDirectoryInfo;

class Record
{
public:
	Record();
	virtual ~Record();

	Leader leader;
	std::list<Directory> directory;
	

public:
	virtual BOOL ReadRecord(DRDirectoryInfo* dir, BYTE*& buf) {	return TRUE; };
	virtual bool WriteRecord(CFile* file) { return true; };
};