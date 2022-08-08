#pragma once
#include "Record.h"
#include "F_IRID.h"

#include <list>

class F_ATTR;
class F_INAS;
class DRDirectoryInfo;
// Information Record
class R_InformationRecord : Record
{
public:
	R_InformationRecord();
	virtual ~R_InformationRecord();

public:
	// 000
	F_IRID m_irid;
	std::list<F_ATTR*> m_attr;
	std::list<F_INAS*> m_inas;

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	bool WriteRecord(CFile* file);

	int GetRCID();
	int GetAssociationCount();
	int GetInformationAssociationCount();
	int GetNumericCode();	
};