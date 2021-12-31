#include "StdAfx.h"
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