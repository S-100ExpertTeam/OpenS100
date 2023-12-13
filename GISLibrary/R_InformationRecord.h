#pragma once

#include "S100_GF.h"
#include "Record.h"
#include "F_IRID.h"

#include <list>

class F_ATTR;
class F_INAS;
class DRDirectoryInfo;
// Information Record
class R_InformationRecord : public GF::InformationType, public Record
{
public:
	R_InformationRecord();
	R_InformationRecord(const R_InformationRecord& other);
	virtual ~R_InformationRecord();

public:
	// 000
	F_IRID m_irid;
	std::list<F_ATTR*> m_attr;
	std::list<F_INAS*> m_inas;

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	bool WriteRecord(CFile* file);
	RecordName GetRecordName();

	int GetRCID();
	std::string GetRCIDAsString(std::string prefix = "");
	int GetAssociationCount();
	int GetInformationAssociationCount();
	int GetNumericCode();	

	std::vector<ATTR*> GetAllAttributes();

	std::vector<ATTR*> GetRootAttributes();
	std::vector<ATTR*> GetRootAttributes(int numericCode);

	std::vector<ATTR*> GetChildAttributes(ATTR* parentATTR);
	std::vector<ATTR*> GetChildAttributes(ATTR* parentATTR, int numericCode);

	std::vector<F_INAS*> GetAllInformationAssociations();

	int GetAttributeIndex(ATTR* attr);

public: // override ObjectType
	std::string GetID() override;
	std::wstring GetIDAsWString() override;
	int GetIDAsInteger() override;
	int GetInformationRelationCount() override;
	std::string GetAssociatedInformationID(int index) override;
	int GetAttributeCount() override;
	std::string GetAttributeValue(int index) override;
	int GetParentAttributeIndex(int index) override;
};