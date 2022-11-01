#include "stdafx.h"
#include "R_DSGIR.h"
#include "F_ATTR.h"
#include "CodeWithNumericCode.h"
#include "DRDirectoryInfo.h"

R_DSGIR::R_DSGIR()
{
	m_atcs = new F_CodeWithNumericCode();
	m_itcs = new F_CodeWithNumericCode();
	m_ftcs = new F_CodeWithNumericCode();
	m_iacs = new F_CodeWithNumericCode();
	m_facs = new F_CodeWithNumericCode();
	m_arcs = new F_CodeWithNumericCode();
}

R_DSGIR::~R_DSGIR()
{
	if (m_atcs)
	{
		delete m_atcs;
	}

	if (m_itcs)
	{
		delete m_itcs;
	}

	if (m_ftcs)
	{
		delete m_ftcs;
	}

	if (m_iacs)
	{
		delete m_iacs;
	}

	if (m_facs)
	{
		delete m_facs;
	}

	if (m_arcs)
	{
		delete m_arcs;
	}
}

#pragma warning(disable:4018)
BOOL R_DSGIR::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
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