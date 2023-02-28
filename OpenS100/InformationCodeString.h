#pragma once

#include "../GISLibrary/S100Interface.h"

#include <string>

class R_InformationRecord;

class CInformationCodeString
{
public:
	int _id;
	std::wstring _name;
	S100Interface::InformationType* _ir;

public:
	bool operator < (CInformationCodeString& value);
};