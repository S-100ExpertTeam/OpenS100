#pragma once
#include <string>

class R_InformationRecord;
class CInformationCodeString
{
public:
	int _id;
	std::wstring _name;
	R_InformationRecord* _ir;

public:
	bool operator < (CInformationCodeString& value);
};