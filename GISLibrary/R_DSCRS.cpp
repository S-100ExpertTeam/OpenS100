#include "stdafx.h"
#include "R_DSCRS.h"
#include "DRDirectoryInfo.h"
#include "F_VDAT.h"
#include "F_CRSH.h"
#include "F_CSAX.h"
#include "F_PROJ.h"
#include "F_GDAT.h"

R_DSCRS::R_DSCRS()
{

}

R_DSCRS::~R_DSCRS()
{
	for (auto itor = m_crsh.begin(); itor != m_crsh.end(); itor++)
	{
		delete *itor;
	}
	delete m_csax;
	delete m_proj;
	delete m_gdat;
	delete m_vdat;
}

#pragma warning(disable:4244)
#pragma warning(disable:4018)
BOOL R_DSCRS::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	USES_CONVERSION;
	for(unsigned i = 0; i < dir->m_count; i++)
	{
		if(dir->GetDirectory(i)->tag == *((unsigned int*)"CSID"))
		{
			m_csid.ReadField(buf);
		}
		else if(dir->GetDirectory(i)->tag == *((unsigned int*)"CRSH"))
		{
			F_CRSH *crsh = new F_CRSH();
			crsh->ReadField(buf);
			m_crsh.push_back(crsh);
		}
		else if(dir->GetDirectory(i)->tag == *((unsigned int*)"CSAX"))
		{
			if (!m_csax) m_csax = new F_CSAX();
			m_csax->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"PROJ"))
		{
			if (!m_proj) m_proj = new F_PROJ();
			m_proj->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"GDAT"))
		{
			if (!m_gdat) m_gdat = new F_GDAT();
			m_gdat->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"VDAT"))
		{
			if (!m_vdat) m_vdat = new F_VDAT();
			m_vdat->ReadField(buf);
		}
		else
		{
			buf += dir->GetDirectory(i)->length;
		}
		if(*(buf++)!= 0x1E)//{}
			TRACE(W2A(TEXT("terminator error")));
	}
	return TRUE;
}

bool R_DSCRS::WriteRecord(CFile* file)
{
	// Set directory
	int fieldOffset = 0;
	int fieldLength = m_csid.GetFieldLength();
	Directory dirFRID("CSID", fieldLength, fieldOffset);
	directory.push_back(dirFRID);
	fieldOffset += fieldLength;

	for (auto i = m_crsh.begin(); i != m_crsh.end(); i++)
	{
		fieldLength = (*i)->GetFieldLength();
		Directory dir("CRSH", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	if (m_csax)
	{
		fieldLength = m_csax->GetFieldLength();
		Directory dirFOID("CSAX", fieldLength, fieldOffset);
		directory.push_back(dirFOID);
		fieldOffset += fieldLength;
	}

	if (m_vdat)
	{
		fieldLength = m_vdat->GetFieldLength();
		Directory dirFOID("VDAT", fieldLength, fieldOffset);
		directory.push_back(dirFOID);
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
	m_csid.WriteField(file);

	for (auto i = m_crsh.begin(); i != m_crsh.end(); i++)
	{
		(*i)->WriteField(file);
	}

	if (m_csax)
	{
		m_csax->WriteField(file);
	}

	if (m_vdat)
	{
		m_vdat->WriteField(file);
	}

	return true;
}
