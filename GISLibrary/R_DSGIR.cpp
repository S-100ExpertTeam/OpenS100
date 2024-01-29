#include "stdafx.h"
#include "R_DSGIR.h"
#include "F_ATTR.h"
#include "CodeWithNumericCode.h"
#include "DRDirectoryInfo.h"

R_DSGIR::R_DSGIR()
{
}

R_DSGIR::~R_DSGIR()
{
	deleteAll();
}

#pragma warning(disable:4018)
BOOL R_DSGIR::ReadRecord(DRDirectoryInfo* dir, BYTE*& buf)
{
	//USES_CONVERSION;
	for (unsigned i = 0; i < dir->m_count; i++)
	{
		if (strcmp(dir->GetDirectory(i)->tag, "DSID") == 0)
		{
			m_dsid.ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "DSSI") == 0)
		{
			m_dssi.ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "ATCS") == 0)
		{
			if (nullptr == m_atcs)
			{
				m_atcs = new F_CodeWithNumericCode();
			}

			m_atcs->ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "ITCS") == 0)
		{
			if (nullptr == m_itcs)
			{
				m_itcs = new F_CodeWithNumericCode();
			}

			m_itcs->ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "FTCS") == 0)
		{
			if (nullptr == m_ftcs)
			{
				m_ftcs = new F_CodeWithNumericCode();
			}

			m_ftcs->ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "IACS") == 0)
		{
			if (nullptr == m_iacs)
			{
				m_iacs = new F_CodeWithNumericCode();
			}

			m_iacs->ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "FACS") == 0)
		{
			if (nullptr == m_facs)
			{
				m_facs = new F_CodeWithNumericCode();
			}

			m_facs->ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "ARCS") == 0)
		{
			if (nullptr == m_arcs)
			{
				m_arcs = new F_CodeWithNumericCode();
			}

			m_arcs->ReadField(buf);
		}
		else
		{
			buf += dir->GetDirectory(i)->length;
		}

		if (*(buf++) != 0x1E)
		{
		}
	}
	return true;
}

bool R_DSGIR::WriteRecord(CFile* file)
{
	directory.clear();

	// Set directory
	int fieldOffset = 0;
	int fieldLength = m_dsid.GetFieldLength();
	Directory dirDSID("DSID", fieldLength, fieldOffset);
	directory.push_back(dirDSID);
	fieldOffset += fieldLength;

	fieldLength = m_dssi.GetFieldLength();
	Directory dirDSSI("DSSI", fieldLength, fieldOffset);
	directory.push_back(dirDSSI);
	fieldOffset += fieldLength;

	if (m_atcs)
	{
		fieldLength = m_atcs->GetFieldLength();
		Directory dir("ATCS", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	if (m_itcs)
	{
		fieldLength = m_itcs->GetFieldLength();
		Directory dir("ITCS", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	if (m_ftcs)
	{
		fieldLength = m_ftcs->GetFieldLength();
		Directory dir("FTCS", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	if (m_iacs)
	{
		fieldLength = m_iacs->GetFieldLength();
		Directory dir("IACS", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	if (m_facs)
	{
		fieldLength = m_facs->GetFieldLength();
		Directory dir("FACS", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	if (m_arcs)
	{
		fieldLength = m_arcs->GetFieldLength();
		Directory dir("ARCS", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	int totalFieldSize = fieldOffset;

	// Set leader
	SetLeader(totalFieldSize);
	leader.SetAsDR();
	leader.WriteLeader(file);

	// Write directory
	WriteDirectory(file);

	// Write field area
	m_dsid.WriteField(file);
	m_dssi.WriteField(file);

	if (m_atcs)
	{
		m_atcs->WriteField(file);
	}

	if (m_itcs)
	{
		m_itcs->WriteField(file);
	}

	if (m_ftcs)
	{
		m_ftcs->WriteField(file);
	}

	if (m_iacs)
	{
		m_iacs->WriteField(file);
	}

	if (m_facs)
	{
		m_facs->WriteField(file);
	}

	if (m_arcs)
	{
		m_arcs->WriteField(file);
	}

	return true;
}


CString R_DSGIR::GetFeatureCode(int numericCode)
{
	auto item = m_ftcs->m_arr.find(numericCode);
	if (item != m_ftcs->m_arr.end())
	{
		CodeWithNumericCode* nc = item->second;
		return nc->m_code;
	}
	return L"";
}


CString R_DSGIR::GetInformationCode(int numericCode)
{
	auto item = m_itcs->m_arr.find(numericCode);
	if (item != m_itcs->m_arr.end())
	{
		CodeWithNumericCode* nc = item->second;
		return nc->m_code;
	}
	return L"";
}


CString R_DSGIR::GetAttributeCode(int numericCode)
{
	auto item = m_atcs->m_arr.find(numericCode);
	if (item != m_atcs->m_arr.end())
	{
		CodeWithNumericCode* nc = item->second;
		return nc->m_code;
	}
	return L"";
}


CString R_DSGIR::GetInformationAssociationCode(int numericCode)
{
	auto item = m_iacs->m_arr.find(numericCode);
	if (item != m_iacs->m_arr.end())
	{
		CodeWithNumericCode* nc = item->second;
		return nc->m_code;
	}
	return L"";
}


CString R_DSGIR::GetFeatureAssociationCode(int numericCode)
{
	auto item = m_facs->m_arr.find(numericCode);
	if (item != m_facs->m_arr.end())
	{
		CodeWithNumericCode* nc = item->second;
		return nc->m_code;
	}
	return L"";
}


CString R_DSGIR::GetAssociationRoleCode(int numericCode)
{
	auto item = m_arcs->m_arr.find(numericCode);
	if (item != m_arcs->m_arr.end())
	{
		CodeWithNumericCode* nc = item->second;
		return nc->m_code;
	}
	return L"";
}

int R_DSGIR::GetAttributeCode(std::wstring& value)
{
	CString str = value.c_str();
	return GetAttributeCode(str);
}

int R_DSGIR::GetAttributeCode(CString& value)
{
	if (nullptr == m_atcs)
	{
		m_atcs = new F_CodeWithNumericCode();
	}

	return m_atcs->GetNumericCode(value);
}

int R_DSGIR::GetInformationTypeCode(std::wstring& value)
{
	CString str = value.c_str();
	return GetInformationTypeCode(str);
}

int R_DSGIR::GetInformationTypeCode(CString& value)
{
	if (nullptr == m_itcs)
	{
		m_itcs = new F_CodeWithNumericCode();
	}

	return m_itcs->GetNumericCode(value);
}

int R_DSGIR::GetFeatureTypeCode(std::wstring& value)
{
	CString str = value.c_str();
	return GetFeatureTypeCode(str);
}

int R_DSGIR::GetFeatureTypeCode(CString& value)
{
	if (nullptr == m_ftcs)
	{
		m_ftcs = new F_CodeWithNumericCode();
	}

	return m_ftcs->GetNumericCode(value);
}

int R_DSGIR::GetInformationAssociationCode(std::wstring& value)
{
	CString str = value.c_str();
	return GetInformationAssociationCode(str);
}

int R_DSGIR::GetInformationAssociationCode(CString& value)
{
	if (nullptr == m_iacs)
	{
		m_iacs = new F_CodeWithNumericCode();
	}

	return m_iacs->GetNumericCode(value);
}

int R_DSGIR::GetFeatureAssociationCode(std::wstring& value)
{
	CString str = value.c_str();
	return GetFeatureAssociationCode(str);
}

int R_DSGIR::GetFeatureAssociationCode(CString& value)
{
	if (nullptr == m_facs)
	{
		m_facs = new F_CodeWithNumericCode();
	}

	return m_facs->GetNumericCode(value);
}

int R_DSGIR::GetAssociationRoleCode(std::wstring& value)
{
	CString str = value.c_str();
	return GetAssociationRoleCode(str);
}

int R_DSGIR::GetAssociationRoleCode(CString& value)
{
	if (nullptr == m_arcs)
	{
		m_arcs = new F_CodeWithNumericCode();
	}

	return m_arcs->GetNumericCode(value);
}

void R_DSGIR::setAttributeCode(int numericCode, CString code)
{
	if (m_atcs) {
		m_atcs->setCodeNumericCode(code, numericCode);
	}
}

void R_DSGIR::setInformationTypeCode(int numericCode, CString code)
{
	if (m_itcs) {
		m_itcs->setCodeNumericCode(code, numericCode);
	}
}

void R_DSGIR::setFeatureTypeCode(int numericCode, CString code)
{
	if (m_ftcs) {
		m_ftcs->setCodeNumericCode(code, numericCode);
	}
}

void R_DSGIR::setInformationAssociationCode(int numericCode, CString code)
{
	if (m_iacs) {
		m_iacs->setCodeNumericCode(code, numericCode);
	}
}

void R_DSGIR::setFeatureAssociationCode(int numericCode, CString code)
{
	if (m_facs) {
		m_facs->setCodeNumericCode(code, numericCode);
	}
}

void R_DSGIR::setAssociationRoleCode(int numericCode, CString code)
{
	if (m_arcs) {
		m_arcs->setCodeNumericCode(code, numericCode);
	}
}

void R_DSGIR::deleteAll()
{
	deleteAttributeCode();
	deleteInformationTypeCode();
	deleteFeatureTypeCode();
	deleteInformationAssociationCode();
	deleteFeatureAssociationCode();
	deleteAssociationRoleCode();
}

void R_DSGIR::deleteAttributeCode()
{
	if (m_atcs) {
		delete m_atcs;
		m_atcs = nullptr;
	}
}

void R_DSGIR::deleteInformationTypeCode()
{
	if (m_itcs) {
		delete m_itcs;
		m_itcs = nullptr;
	}
}

void R_DSGIR::deleteFeatureTypeCode()
{
	if (m_ftcs) {
		delete m_ftcs;
		m_ftcs = nullptr;
	}
}

void R_DSGIR::deleteInformationAssociationCode()
{
	if (m_iacs) {
		delete m_iacs;
		m_iacs = nullptr;
	}
}

void R_DSGIR::deleteFeatureAssociationCode()
{
	if (m_facs) {
		delete m_facs;
		m_facs = nullptr;
	}
}

void R_DSGIR::deleteAssociationRoleCode()
{
	if (m_arcs) {
		delete m_arcs;
		m_arcs = nullptr;
	}
}

void R_DSGIR::init()
{
	initAttributeCode();
	initInformationTypeCode();
	initFeatureTypeCode();
	initInformationAssociationCode();
	initFeatureAssociationCode();
	initAssociationRoleCode();
}

void R_DSGIR::initAttributeCode()
{
	deleteAttributeCode();

	m_atcs = new F_CodeWithNumericCode();
}

void R_DSGIR::initInformationTypeCode()
{
	deleteInformationTypeCode();

	m_itcs = new F_CodeWithNumericCode();
}

void R_DSGIR::initFeatureTypeCode()
{
	deleteFeatureTypeCode();

	m_ftcs = new F_CodeWithNumericCode();
}

void R_DSGIR::initInformationAssociationCode()
{
	deleteInformationAssociationCode();

	m_iacs = new F_CodeWithNumericCode();
}

void R_DSGIR::initFeatureAssociationCode()
{
	deleteFeatureAssociationCode();

	m_facs = new F_CodeWithNumericCode();
}

void R_DSGIR::initAssociationRoleCode()
{
	deleteAssociationRoleCode();

	m_arcs = new F_CodeWithNumericCode();
}

R_DSGIR* R_DSGIR::Clone() const
{
	R_DSGIR* r_dsgir = new R_DSGIR();

	if (r_dsgir->m_atcs)
	{
		delete r_dsgir->m_atcs;
		r_dsgir->m_atcs = nullptr;
	}

	if (r_dsgir->m_itcs)
	{
		delete r_dsgir->m_itcs;
		r_dsgir->m_itcs = nullptr;
	}

	if (r_dsgir->m_ftcs)
	{
		delete r_dsgir->m_ftcs;
		r_dsgir->m_ftcs = nullptr;
	}

	if (r_dsgir->m_iacs)
	{
		delete r_dsgir->m_iacs;
		r_dsgir->m_iacs = nullptr;
	}

	if (r_dsgir->m_facs)
	{
		delete r_dsgir->m_facs;
		r_dsgir->m_facs = nullptr;
	}

	if (r_dsgir->m_arcs)
	{
		delete r_dsgir->m_arcs;
		r_dsgir->m_arcs = nullptr;
	}

	r_dsgir->m_dsid.m_name = m_dsid.m_name;
	r_dsgir->m_dsid.m_ensp = m_dsid.m_ensp;
	r_dsgir->m_dsid.m_ened = m_dsid.m_ened;
	r_dsgir->m_dsid.m_prsp = m_dsid.m_prsp;
	r_dsgir->m_dsid.m_pred = m_dsid.m_pred;
	r_dsgir->m_dsid.m_prof = m_dsid.m_prof;
	r_dsgir->m_dsid.m_dsnm = m_dsid.m_dsnm;
	r_dsgir->m_dsid.m_dstl = m_dsid.m_dstl;
	r_dsgir->m_dsid.m_dsrd = m_dsid.m_dsrd;
	r_dsgir->m_dsid.m_dslg = m_dsid.m_dslg;
	r_dsgir->m_dsid.m_dsab = m_dsid.m_dsab;
	r_dsgir->m_dsid.m_dsed = m_dsid.m_dsed;
	for (int i = 0; i < m_dsid.m_dstc.GetSize(); i++)
		r_dsgir->m_dsid.m_dstc.Add(m_dsid.m_dstc[i]);

	r_dsgir->m_dssi.m_dcox = m_dssi.m_dcox;
	r_dsgir->m_dssi.m_dcoy = m_dssi.m_dcoy;
	r_dsgir->m_dssi.m_dcoz = m_dssi.m_dcoz;
	r_dsgir->m_dssi.m_cmfx = m_dssi.m_cmfx;
	r_dsgir->m_dssi.m_cmfy = m_dssi.m_cmfy;
	r_dsgir->m_dssi.m_cmfz = m_dssi.m_cmfz;
	r_dsgir->m_dssi.m_noir = m_dssi.m_noir;
	r_dsgir->m_dssi.m_nopn = m_dssi.m_nopn;
	r_dsgir->m_dssi.m_nomn = m_dssi.m_nomn;
	r_dsgir->m_dssi.m_nocn = m_dssi.m_nocn;
	r_dsgir->m_dssi.m_noxn = m_dssi.m_noxn;
	r_dsgir->m_dssi.m_nosn = m_dssi.m_nosn;
	r_dsgir->m_dssi.m_nofr = m_dssi.m_nofr;

	r_dsgir->m_atcs = (!m_atcs) ? nullptr : m_atcs->Clone();
	r_dsgir->m_itcs = (!m_atcs) ? nullptr : m_atcs->Clone();
	r_dsgir->m_ftcs = (!m_atcs) ? nullptr : m_atcs->Clone();
	r_dsgir->m_iacs = (!m_atcs) ? nullptr : m_atcs->Clone();
	r_dsgir->m_facs = (!m_atcs) ? nullptr : m_atcs->Clone();
	r_dsgir->m_arcs = (!m_atcs) ? nullptr : m_atcs->Clone();

	return r_dsgir;
}
