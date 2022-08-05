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

bool R_DSGIR::WriteRecord(CFile* file)
{
	//DRReaderWriter dr;
	//DRDirectoryInfoWriter dirInfo;
	//DRDirectory* dir;
	//unsigned rLen = 0;

	//int dirInfoCnt = 0;

	//dirInfoCnt++;

	//dirInfoCnt++;

	//if (m_atcs)
	//{
	//	dirInfoCnt++;
	//}

	//if (m_itcs)
	//{
	//	dirInfoCnt++;
	//}

	//if (m_ftcs)
	//{
	//	dirInfoCnt++;
	//}

	//if (m_iacs)
	//{
	//	dirInfoCnt++;
	//}

	//if (m_facs)
	//{
	//	dirInfoCnt++;
	//}

	//if (m_arcs)
	//{
	//	dirInfoCnt++;
	//}

	////dirInfoCnt += m_attr.size();

	//dirInfo.ReAllocateDirectory(dirInfoCnt);

	//dirInfoCnt = 0;

	//rLen = 0;
	//dir = dirInfo.GetDirectory(dirInfoCnt++);
	//dir->tag = *((unsigned int*)"DSID");
	//dir->pos = rLen;
	//dir->length = m_dsid.GetFieldLength();
	//rLen += dir->length;

	//dir = dirInfo.GetDirectory(dirInfoCnt++);
	//dir->tag = *((unsigned int*)"DSSI");
	//dir->pos = rLen;
	//dir->length = m_dssi.GetFieldLength();
	//rLen += dir->length;


	//if (m_atcs)
	//{
	//	dir = dirInfo.GetDirectory(dirInfoCnt++);
	//	dir->tag = *((unsigned int*)"ATCS");
	//	dir->pos = rLen;
	//	dir->length = m_atcs->GetFieldLength();
	//	rLen += dir->length;
	//}

	//if (m_itcs)
	//{
	//	dir = dirInfo.GetDirectory(dirInfoCnt++);
	//	dir->tag = *((unsigned int*)"ITCS");
	//	dir->pos = rLen;
	//	dir->length = m_itcs->GetFieldLength();
	//	rLen += dir->length;
	//}

	//if (m_ftcs)
	//{
	//	dir = dirInfo.GetDirectory(dirInfoCnt++);
	//	dir->tag = *((unsigned int*)"FTCS");
	//	dir->pos = rLen;
	//	dir->length = m_ftcs->GetFieldLength();
	//	rLen += dir->length;
	//}

	//if (m_iacs)
	//{
	//	dir = dirInfo.GetDirectory(dirInfoCnt++);
	//	dir->tag = *((unsigned int*)"IACS");
	//	dir->pos = rLen;
	//	dir->length = m_iacs->GetFieldLength();
	//	rLen += dir->length;
	//}

	//if (m_facs)
	//{
	//	dir = dirInfo.GetDirectory(dirInfoCnt++);
	//	dir->tag = *((unsigned int*)"FACS");
	//	dir->pos = rLen;
	//	dir->length = m_facs->GetFieldLength();
	//	rLen += dir->length;
	//}

	//if (m_arcs)
	//{
	//	dir = dirInfo.GetDirectory(dirInfoCnt++);
	//	dir->tag = *((unsigned int*)"ARCS");
	//	dir->pos = rLen;
	//	dir->length = m_arcs->GetFieldLength();
	//	rLen += dir->length;
	//}


	////for (auto itor = m_attr.begin(); itor != m_attr.end(); itor++)
	////{
	////	F_ATTR* attr = *itor;
	////	dir = dirInfo.GetDirectory(dirInfoCnt++);
	////	dir->tag = *((unsigned int*)"ATTR");
	////	dir->pos = rLen;
	////	dir->length = attr->GetFieldLength();
	////	rLen += dir->length;
	////}

	//dirInfo.CheckLength(&dr);
	//dr.m_fieldAreaLoc = DRReader::size + (4 + dr.m_fieldLength + dr.m_fieldPosition) * dirInfo.m_count + 1;
	//dr.m_recordLength = dr.m_fieldAreaLoc + rLen;

	////////////////////////////////////////////////
	//dr.WriteDRReader(file);
	//dirInfo.WriteDRDirectory(file, dr);
	//m_dsid.Save(file);
	//m_dssi.Save(file);
	//if (m_atcs)	m_atcs->Save(file);
	//if (m_itcs)	m_itcs->Save(file);
	//if (m_ftcs)	m_ftcs->Save(file);
	//if (m_iacs)	m_iacs->Save(file);
	//if (m_facs)	m_facs->Save(file);
	//if (m_arcs)	m_arcs->Save(file);
	////for (auto itor = m_attr.begin(); itor != m_attr.end(); itor++)
	////{
	////	F_ATTR* attr = *itor;
	////	attr->Save(file);
	////}

	return TRUE;
}