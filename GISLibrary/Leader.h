#pragma once
class Leader
{
public:
	Leader();
	virtual ~Leader();

public:
	int recordLength = 0;
	int baseAddressOfFieldArea = 0;

	// Entry map
	int sizeOfFieldLengthField = 0;
	int sizeOfFieldPositionField = 0;

	char byte[24] = { 0, };

public:
	//bool Read(char*& buf);

	
	int GetRecordLength();
	int GetBaseAddressOfFieldArea();
	int GetSizeOfFieldLengthField();
	int GetSizeOfFieldPositionField();

	void SetAsDR();

	bool WriteLeader(CFile* file);
};