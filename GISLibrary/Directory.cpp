#include "stdafx.h"
#include "Directory.h"

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

bool Directory::WriteDirectory(CFile* file, int sizeOfFieldLengthField, int sizeOfFieldPositionField)
{
	file->Write(fieldTag, 4);

	CString str;
	str.Format(L"%09d", fieldLength);
	CString trimStr = str.Right(sizeOfFieldLengthField);

	unsigned char* byteSizeOfFieldLengthField = new unsigned char[sizeOfFieldLengthField];
	for (int i = 0; i < sizeOfFieldLengthField; i++)
	{
		byteSizeOfFieldLengthField[i] = (unsigned char)trimStr[i];
	}
	file->Write((const void*)&byteSizeOfFieldLengthField[0], sizeOfFieldLengthField);
	delete[] byteSizeOfFieldLengthField;
	
	str.Format(L"%09d", fieldPosition); 
	trimStr = str.Right(sizeOfFieldPositionField);

	unsigned char* byteSizeOfFieldPositionField = new unsigned char[sizeOfFieldPositionField];
	for (int i = 0; i < sizeOfFieldPositionField; i++)
	{
		byteSizeOfFieldPositionField[i] = (unsigned char)trimStr[i];
	}
	file->Write((const void*)&byteSizeOfFieldPositionField[0], sizeOfFieldPositionField);
	delete[] byteSizeOfFieldPositionField;

	return true;
}