#pragma once
#include "BinaryWriter.h"

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
	R_DSGIR(const R_DSGIR& other);
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
	virtual R_DSGIR operator=(const R_DSGIR& other);

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	bool WriteRecord(s100::io::BinaryWriter* file);
	// function to get the code with the new memory code.
	std::wstring GetFeatureCode(int numericCode);
	std::string GetFeatureCodeAsString(int numericCode);
	std::wstring GetInformationCode(int numericCode);
	std::string GetInformationCodeAsString(int numericCode);
	std::wstring GetAttributeCode(int numericCode);
	std::string GetAttributeCodeAsString(int numericCode);
	std::wstring GetInformationAssociationCode(int numericCode);
	std::string GetInformationAssociationCodeAsString(int numericCode);
	std::wstring GetFeatureAssociationCode(int numericCode);
	std::string GetFeatureAssociationCodeAsString(int numericCode);
	std::wstring GetAssociationRoleCode(int numericCode);
	std::string GetAssociationRoleCodeAsString(int numericCode);

	int GetAttributeCode(const std::string& value);
	int GetAttributeCode(const std::wstring& value);

	int GetInformationTypeCode(const std::wstring& value);

	int GetFeatureTypeCode(const std::wstring& value);

	int GetInformationAssociationCode(const std::wstring& value);

	int GetFeatureAssociationCode(const std::wstring& value);

	int GetAssociationRoleCode(const std::wstring& value);

	void setAttributeCode(int numericCode, const std::wstring& code);
	void setInformationTypeCode(int numericCode, const std::wstring& code);
	void setFeatureTypeCode(int numericCode, const std::wstring& code);
	void setInformationAssociationCode(int numericCode, const std::wstring& code);
	void setFeatureAssociationCode(int numericCode, const std::wstring& code);
	void setAssociationRoleCode(int numericCode, const std::wstring& code);

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