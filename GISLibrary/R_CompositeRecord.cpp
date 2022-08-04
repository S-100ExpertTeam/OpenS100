#include "stdafx.h"
#include "R_CompositeRecord.h"
#include "DRDirectoryInfo.h"
#include "F_CCOC.h"
#include "F_CUCO.h"
#include "F_INAS.h"

R_CompositeRecord::R_CompositeRecord(void)
{
	m_ccid.m_name.RCNM = 125;
}

R_CompositeRecord::~R_CompositeRecord(void)
{
	delete m_ccoc;

	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		delete *itor;
	}

	for (auto itor = m_cuco.begin(); itor != m_cuco.end(); itor++)
	{
		delete *itor;
	}
}

BOOL R_CompositeRecord::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	USES_CONVERSION;
	int i = 0, j = 0;
	for(i = 0; i < dir->m_count; i++)
	{
		if(dir->GetDirectory(i)->tag == *((unsigned int*)"CCID"))
		{
			m_ccid.ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"INAS"))
		{
			F_INAS* inas = new F_INAS();

			inas->ReadField(buf);

			m_inas.push_back(inas);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"COCC"))
		{
			if (nullptr == m_ccoc)
			{
				m_ccoc = new F_CCOC();
			}

			m_ccoc->ReadField(buf);
		}
		else if(dir->GetDirectory(i)->tag == *((unsigned int*)"CUCO"))
		{
			F_CUCO* cuco = new F_CUCO();
			cuco->ReadField(buf);
			m_cuco.push_back(cuco);
		}
		else
		{
			buf += dir->GetDirectory(i)->length;
		}
		
		if (*(buf++) != 0x1E)
		{
			TRACE(W2A(TEXT("terminator error")));
		}
	}
	return TRUE;
}

int R_CompositeRecord::GetRCID()
{
	return m_ccid.m_name.RCID;
}

std::wstring R_CompositeRecord::GetRCIDasWstring() 
{
	return std::to_wstring(GetRCID());
}