#include "stdafx.h"
#include "Leader.h"

Leader::Leader()
{

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