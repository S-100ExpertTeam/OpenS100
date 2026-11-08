#include "stdafx.h"
#include "Directory.h"

#include <format>
#include <string>

Directory::Directory()
{

}

Directory::Directory(const Directory& other)
{
	memcpy(fieldTag, other.fieldTag, 4);
	fieldLength = other.fieldLength;
	fieldPosition = other.fieldPosition;
}

Directory::Directory(char fieldTag[5], int fieldLength, int fieldPosition)
{
	memcpy(this->fieldTag, fieldTag, 4);
	this->fieldLength = fieldLength;
	this->fieldPosition = fieldPosition;
}

Directory::~Directory()
{

}

Directory Directory::operator=(const Directory& other)
{
	memset(fieldTag, 0x00, 4);
	memcpy(fieldTag, other.fieldTag, 4);
	fieldLength = other.fieldLength;
	fieldPosition = other.fieldPosition;

	return *this;
}

bool Directory::WriteDirectory(s100::io::BinaryWriter* file, int sizeOfFieldLengthField, int sizeOfFieldPositionField)
{
	file->Write(fieldTag, 4);

	// Zero-padded decimal, truncated to the width declared in the leader (rightmost digits).
	const std::string length = std::format("{:09d}", fieldLength);
	file->Write(length.data() + length.size() - sizeOfFieldLengthField, sizeOfFieldLengthField);

	const std::string position = std::format("{:09d}", fieldPosition);
	file->Write(position.data() + position.size() - sizeOfFieldPositionField, sizeOfFieldPositionField);

	return true;
}