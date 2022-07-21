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
		if (dir->GetDirectory(i)->tag == *((unsigned int*)"DSID"))
		{
			m_dsid.ReadField(buf);
		}
		else if(dir->GetDirectory(i)->tag == *((unsigned int*)"DSSI"))
		{
			m_dssi.ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"ATCS"))
		{
			if (nullptr == m_atcs)
			{
				m_atcs = new F_CodeWithNumericCode();
			}

			m_atcs->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"ITCS"))
		{
			if (nullptr == m_itcs)
			{
				m_itcs = new F_CodeWithNumericCode();
			}

			m_itcs->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"FTCS"))
		{
			if (nullptr == m_ftcs)
			{
				m_ftcs = new F_CodeWithNumericCode();
			}

			m_ftcs->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"IACS"))
		{
			if (nullptr == m_iacs)
			{
				m_iacs = new F_CodeWithNumericCode();
			}

			m_iacs->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"FACS"))
		{
			if (nullptr == m_facs)
			{
				m_facs = new F_CodeWithNumericCode();
			}

			m_facs->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"ARCS"))
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

void R_DSGIR::AddAttributeCode(CString& value)
{
	if (nullptr == m_atcs)
	{
		m_atcs = new F_CodeWithNumericCode();
	}

	m_atcs->AddNewCodeNumericCode(value);
}

void R_DSGIR::AddInformationTypeCode(CString& value)
{
	if (nullptr == m_itcs)
	{
		m_itcs = new F_CodeWithNumericCode();
	}

	m_itcs->AddNewCodeNumericCode(value);
}

void R_DSGIR::AddFeatureTypeCode(CString& value)
{
	if (nullptr == m_ftcs)
	{
		m_ftcs = new F_CodeWithNumericCode();
	}

	m_ftcs->AddNewCodeNumericCode(value);
}

void R_DSGIR::AddInformationAssociationCode(CString& value)
{
	if (nullptr == m_iacs)
	{
		m_iacs = new F_CodeWithNumericCode();
	}

	m_iacs->AddNewCodeNumericCode(value);
}

void R_DSGIR::AddFeatureAssociationCode(CString& value)
{
	if (nullptr == m_facs)
	{
		m_facs = new F_CodeWithNumericCode();
	}

	m_facs->AddNewCodeNumericCode(value);
}

void R_DSGIR::AddAssociationRoleCode(CString& value)
{
	if (nullptr == m_arcs)
	{
		m_arcs = new F_CodeWithNumericCode();
	}

	m_arcs->AddNewCodeNumericCode(value);
}