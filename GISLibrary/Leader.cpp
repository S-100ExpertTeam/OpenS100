#include "stdafx.h"
#include "Leader.h"

Leader::Leader()
{

}

Leader::Leader(const Leader& other)
{
	recordLength = other.recordLength;
	baseAddressOfFieldArea = other.baseAddressOfFieldArea;
		
	sizeOfFieldLengthField = other.sizeOfFieldLengthField;
	sizeOfFieldPositionField = other.sizeOfFieldPositionField;

	memcpy(byte, other.byte, 24);
}

Leader::~Leader()
{

}

int Leader::GetRecordLength()
{
	return recordLength;
}

int Leader::GetBaseAddressOfFieldArea()
{
	return baseAddressOfFieldArea;
}

int Leader::GetSizeOfFieldLengthField()
{
	return sizeOfFieldLengthField;
}

int Leader::GetSizeOfFieldPositionField()
{
	return sizeOfFieldPositionField;
}

void Leader::SetAsDDR()
{
	// Record length
	CString str;
	str.Format(L"%05d", recordLength);
	for (int i = 0; i < 5; i++)
	{
		byte[i] = (unsigned char)str[i];
	}

	byte[5] = 0x33; // "3"
	byte[6] = 0x4C; // "L"
	byte[7] = 0x45; // "E"
	byte[8] = 0x31; // "1"
	byte[9] = 0x20; // SPACE

	// Field control length "09"
	byte[10] = 0x30;  
	byte[11] = 0x39; 

	// Base address of field area
	str.Format(L"%05d", baseAddressOfFieldArea);
	for (int i = 0; i < 5; i++)
	{
		byte[12 + i] = (unsigned char)str[i];
	}

	// Extended character set indicator
	byte[17] = 0x20; // SPACE
	byte[18] = 0x21; // "1"
	byte[19] = 0x20; // SPACE

	str.Format(L"%01d", sizeOfFieldLengthField);
	byte[20] = (unsigned char)str[0];

	str.Format(L"%01d", sizeOfFieldPositionField);
	byte[21] = (unsigned char)str[0];

	byte[22] = 0x30; // "0"
	byte[23] = 0x34; // "4"
}

void Leader::SetAsDR()
{
	// Record length
	CString str;
	str.Format(L"%05d", recordLength);
	for (int i = 0; i < 5; i++)
	{
		byte[i] = (unsigned char)str[i];
	}

	byte[5] = 0x20; // SPACE
	byte[6] = 0x44; // "D"
	byte[7] = 0x20; // SPACE
	byte[8] = 0x20; // SPACE
	byte[9] = 0x20; // SPACE
	
	// Field control length
	byte[10] = 0x20; // SPACE
	byte[11] = 0x20; // SPACE

	// Base address of field area
	str.Format(L"%05d", baseAddressOfFieldArea);
	for (int i = 0; i < 5; i++)
	{
		byte[12 + i] = (unsigned char)str[i];
	}

	// Extended character set indicator
	byte[17] = 0x20; // SPACE
	byte[18] = 0x20; // SPACE
	byte[19] = 0x20; // SPACE

	str.Format(L"%01d", sizeOfFieldLengthField);
	byte[20] = (unsigned char)str[0];

	str.Format(L"%01d", sizeOfFieldPositionField);
	byte[21] = (unsigned char)str[0];

	byte[22] = 0x30; // "0"
	byte[23] = 0x34; // "4"
}

bool Leader::WriteLeader(CFile* file)
{
	file->Write(byte, 24);
	return true;
}