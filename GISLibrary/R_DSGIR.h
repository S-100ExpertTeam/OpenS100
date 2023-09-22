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
	F_CodeWithNumericCode *m_atcs = new F_CodeWithNumericCode();
	F_CodeWithNumericCode *m_itcs = new F_CodeWithNumericCode();
	F_CodeWithNumericCode *m_ftcs = new F_CodeWithNumericCode();
	F_CodeWithNumericCode *m_iacs = new F_CodeWithNumericCode();
	F_CodeWithNumericCode *m_facs = new F_CodeWithNumericCode();
	F_CodeWithNumericCode *m_arcs = new F_CodeWithNumericCode();

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

	void setAttributeCode(int numericCode, CString code);
	void setInformationTypeCode(int numericCode, CString code);
	void setFeatureTypeCode(int numericCode, CString code);
	void setInformationAssociationCode(int numericCode, CString code);
	void setFeatureAssociationCode(int numericCode, CString code);
	void setAssociationRoleCode(int numericCode, CString code);

	void init();
	void initAttributeCode();
	void initInformationTypeCode();
	void initFeatureTypeCode();
	void initInformationAssociationCode();
	void initFeatureAssociationCode();
	void initAssociationRoleCode();

private:
	void deleteAll();
	void deleteAttributeCode();
	void deleteInformationTypeCode();
	void deleteFeatureTypeCode();
	void deleteInformationAssociationCode();
	void deleteFeatureAssociationCode();
	void deleteAssociationRoleCode();
};