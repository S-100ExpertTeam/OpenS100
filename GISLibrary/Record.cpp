#include "stdafx.h"
#include "Record.h"
#include "NonPrintableCharacter.h"

#include "../LatLonUtility/LatLonUtility.h"

Record::Record(void)
{

}

Record::Record(const Record& other)
{
	leader = other.leader;
	for (const auto& iter : other.directory)
	{
		Directory dir = iter;
		directory.push_back(dir);
	}
}

Record::~Record(void)
{

}

bool Record::WriteDirectory(CFile* file)
{
	for (auto i = directory.begin(); i != directory.end(); i++)
	{
		i->WriteDirectory(file, leader.GetSizeOfFieldLengthField(), leader.GetSizeOfFieldPositionField());
	}

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

void Record::SetLeader(int totalFieldSize, bool adjustEntryMap)
{
	int maxFieldLength = 0;
	int maxFieldPosition = 0;

	for (auto i = directory.begin(); i != directory.end(); i++)
	{
		if (maxFieldLength < i->fieldLength)
		{
			maxFieldLength = i->fieldLength;
		}

		if (maxFieldPosition < i->fieldPosition)
		{
			maxFieldPosition = i->fieldPosition;
		}
	}

	leader.sizeOfFieldLengthField = (int)LatLonUtility::countDigits(maxFieldLength);
	leader.sizeOfFieldPositionField = (int)LatLonUtility::countDigits(maxFieldPosition);

	if (adjustEntryMap)
	{

		// For compatibility
		if (leader.sizeOfFieldLengthField == 1)
		{
			leader.sizeOfFieldLengthField = 2;
		}

		if (leader.sizeOfFieldPositionField == 1)
		{
			leader.sizeOfFieldPositionField = 2;
		}

	}

	int directoryLength = DirectoryLength(leader.sizeOfFieldLengthField, leader.sizeOfFieldPositionField);
	leader.baseAddressOfFieldArea = directoryLength + 24;

	leader.recordLength = leader.baseAddressOfFieldArea + totalFieldSize;

	
}

int Record::DirectoryLength(int sizeOfFieldLengthField, int sizeOfFieldPositionField)
{
	return (int)(directory.size() * (4 + sizeOfFieldLengthField + sizeOfFieldPositionField) + 1);
}