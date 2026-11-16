#pragma once
#include <cstdint>
#include "BinaryWriter.h"

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

	virtual bool ReadRecord(DRDirectoryInfo* /*dir*/, std::uint8_t*& /*buf*/) {	return true; };
	virtual bool WriteRecord(s100::io::BinaryWriter* /*file*/) { return true; };
	virtual RecordName GetRecordName() { return RecordName(); };

	bool WriteDirectory(s100::io::BinaryWriter* file);
	void SetLeader(int totalFieldSize, bool adjustEntryMap = true);
	int DirectoryLength(int sizeOfFieldLengthField, int sizeOfFieldPositionField);
};