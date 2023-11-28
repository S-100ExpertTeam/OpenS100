#pragma once

#include "../GISLibrary/S100_GF.h"

#include <string>

class R_InformationRecord;

class CInformationCodeString
{
public:
	CString _id;
	std::wstring _name;
	GF::InformationType* _ir;

public:
	bool operator < (CInformationCodeString& value);
};