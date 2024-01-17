#pragma once
class Leader
{
public:
	Leader();
	Leader(const Leader& other);
	virtual ~Leader();

public:
	int recordLength = 0;
	int baseAddressOfFieldArea = 0;

	// Entry map
	int sizeOfFieldLengthField = 0;
	int sizeOfFieldPositionField = 0;

	char byte[24] = { 0, };

public:
	virtual Leader operator=(const Leader& other);

public:
	//bool Read(char*& buf);
	int GetRecordLength();
	int GetBaseAddressOfFieldArea();
	int GetSizeOfFieldLengthField();
	int GetSizeOfFieldPositionField();

	void SetAsDDR();
	void SetAsDR();	

	bool WriteLeader(CFile* file);
};