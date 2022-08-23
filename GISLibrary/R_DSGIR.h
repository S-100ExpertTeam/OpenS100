#pragma once

#include "Record.h"
#include "F_DSID.h"
#include "F_DSSI.h"
#include "F_CodeWithNumericCode.h"

class F_ATTR;
class DRDirectoryInfo;
// Dataset General Information Record
class R_DSGIR : Record
{
public:
	R_DSGIR();
	virtual ~R_DSGIR();

public:
	F_DSID m_dsid;
	F_DSSI m_dssi;
	F_CodeWithNumericCode *m_atcs = nullptr;
	F_CodeWithNumericCode *m_itcs = nullptr;
	F_CodeWithNumericCode *m_ftcs = nullptr;
	F_CodeWithNumericCode *m_iacs = nullptr;
	F_CodeWithNumericCode *m_facs = nullptr;
	F_CodeWithNumericCode *m_arcs = nullptr;

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	bool WriteRecord(CFile* file);
	// function to get the code with the new memory code.
	CString GetFeatureCode(int numericCode);
	CString GetInformationCode(int numericCode);
	CString GetAttributeCode(int numericCode);
	CString GetInformationAssociationCode(int numericCode);
	CString GetFeatureAssociationCode(int numericCode);
	CString GetAssociationRoleCode(int numericCode);

	int GetAttributeCode(std::wstring& value);
	int GetAttributeCode(CString& value);

	int GetInformationTypeCode(std::wstring& value);
	int GetInformationTypeCode(CString& value);

	int GetFeatureTypeCode(std::wstring& value);
	int GetFeatureTypeCode(CString& value);

	int GetInformationAssociationCode(std::wstring& value);
	int GetInformationAssociationCode(CString& value);

	int GetFeatureAssociationCode(std::wstring& value);
	int GetFeatureAssociationCode(CString& value);

	int GetAssociationRoleCode(std::wstring& value);
	int GetAssociationRoleCode(CString& value);
};