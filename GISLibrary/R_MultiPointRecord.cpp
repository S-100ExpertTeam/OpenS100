#include "stdafx.h"
#include "R_MultiPointRecord.h"
#include "DRDirectoryInfo.h"
#include "F_C2IL.h"
#include "F_COCC.h"
#include "F_C3IL.h"
#include "F_INAS.h"
#include "IC2D.h"
#include "C3IL.h"

R_MultiPointRecord::R_MultiPointRecord(void)
{

}

R_MultiPointRecord::~R_MultiPointRecord(void)
{
	delete m_cocc;
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		delete *itor;
	}

	for (auto itor = m_c3il.begin(); itor != m_c3il.end(); itor++)
	{
		delete *itor;
	}

	for (auto itor = m_c2il.begin(); itor != m_c2il.end(); itor++)
	{
		delete *itor;
	}
}

#pragma warning(disable:4018)
BOOL R_MultiPointRecord::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	USES_CONVERSION;
	int i = 0, j = 0, cnt;
	for(unsigned i = 0; i < dir->m_count; i++)
	{
		if(dir->GetDirectory(i)->tag == *((unsigned int*)"MRID"))
		{
			m_mrid.ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"INAS"))
		{
			F_INAS *inas = new F_INAS();
			inas->ReadField(buf);
			m_inas.push_back(inas);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"COCC"))
		{
			if (!m_cocc)
			{
				m_cocc = new F_COCC();
			}

			cnt = (dir->GetDirectory(i)->length - 1) / F_INAS::GetSize();
			m_cocc->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"C2IL"))
		{
			F_C2IL* c2il = new F_C2IL();
			cnt = (dir->GetDirectory(i)->length - 1) / IC2D::GetSize();
			c2il->ReadField(buf, cnt);
			m_c2il.push_back(c2il);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"C3IL"))
		{
			F_C3IL* c3il = new F_C3IL();
			cnt = (dir->GetDirectory(i)->length - 1) / C3IL::GetSize();
			c3il->ReadField(buf, cnt);
			m_c3il.push_back(c3il);
		}
		else
		{
			buf += dir->GetDirectory(i)->length;
		}
		
		if (*(buf++) != 0x1E)
		{

		}
	}
	return TRUE;
}

int R_MultiPointRecord::GetRCID()
{
	return m_mrid.m_name.RCID;
}

std::wstring R_MultiPointRecord::GetRCIDasWstring()
{
	return std::to_wstring(GetRCID());
}