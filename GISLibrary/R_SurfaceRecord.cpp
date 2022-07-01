#include "StdAfx.h"
#include "R_SurfaceRecord.h"
#include "DRDirectoryInfo.h"
#include "F_INAS.h"
#include "F_RIAS.h"
#include "RIAS.h"

R_SurfaceRecord::R_SurfaceRecord(void)
{
	m_srid.m_name.RCNM = 130;
}

R_SurfaceRecord::~R_SurfaceRecord(void)
{
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		delete *itor;
	}
	for (auto itor = m_rias.begin(); itor != m_rias.end(); itor++)
	{
		delete *itor;
	}
}

BOOL R_SurfaceRecord::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	USES_CONVERSION;
	for(int i = 0; i < dir->m_count; i++)
	{
		if(dir->GetDirectory(i)->tag == *((unsigned int*)"SRID"))
		{
			m_srid.ReadField(buf);
		}
		else if(dir->GetDirectory(i)->tag == *((unsigned int*)"INAS"))
		{
			F_INAS* inas = new F_INAS();
			inas->ReadField(buf);
			m_inas.push_back(inas);
		}
		else if(dir->GetDirectory(i)->tag == *((unsigned int*)"RIAS"))
		{
			F_RIAS* rias = new F_RIAS();
			auto cnt = (dir->GetDirectory(i)->length - 1) / RIAS::GetSize();
			rias->ReadField(buf, cnt);
			m_rias.push_back(rias);
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

int R_SurfaceRecord::GetRCID() 
{
	return m_srid.m_name.RCID;
}

std::wstring R_SurfaceRecord::GetRCIDasWstring() 
{
	return std::to_wstring(GetRCID());
}