#include "StdAfx.h"
#include "R_CurveRecord.h"
#include "DRDirectoryInfo.h"
#include "F_PTAS.h"
#include "F_SEGH.h"
#include "F_SECC.h"
#include "F_COCC.h"
#include "F_C2IL.h"
#include "F_INAS.h"
#include "IC2D.h"
#include "PTAS.h"

R_CurveRecord::R_CurveRecord()
{
	m_crid.m_name.RCNM = 120;
}

R_CurveRecord::~R_CurveRecord()
{
	delete m_ptas;
	m_ptas = nullptr;

	delete m_secc;
	m_secc = nullptr;

	delete m_cocc;
	m_cocc = nullptr;

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_inas.clear();

	for (auto i = m_segh.begin(); i != m_segh.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_segh.clear();

	for (auto i = m_c2il.begin(); i != m_c2il.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_c2il.clear();
}

#pragma warning(disable:4018)
BOOL R_CurveRecord::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	for (int i = 0; i < dir->m_count; i++)
	{
		if (dir->GetDirectory(i)->tag == *((unsigned int*)"CRID"))
		{
			m_crid.ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"INAS"))
		{
			F_INAS* inas = new F_INAS();
			inas->ReadField(buf);
			m_inas.push_back(inas);

			OutputDebugString(L"@@@ INAS @@@ \n\n");
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"PTAS"))
		{
			if (nullptr == m_ptas)
			{
				m_ptas = new F_PTAS();
			}

			auto cnt = (dir->GetDirectory(i)->length  - 1) / PTAS::GetSize();

			m_ptas->ReadField(buf, cnt);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"SECC"))
		{
			if (nullptr == m_secc)
			{
				m_secc = new F_SECC();
			}

			m_secc->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"SEGH"))
		{
			F_SEGH* segh = new F_SEGH();

			segh->ReadField(buf);

			m_segh.push_back(segh);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"COCC"))
		{
			if (nullptr == m_cocc)
			{
				m_cocc = new F_COCC();
			}

			m_cocc->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"C2IL"))
		{
			F_C2IL *c2il = new F_C2IL();

			auto cnt = (dir->GetDirectory(i)->length - 1) / IC2D::GetSize();

			c2il->ReadField(buf, cnt);

			m_c2il.push_back(c2il);
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

int R_CurveRecord::GetRCID() 
{
	return m_crid.m_name.RCID;
}

std::wstring R_CurveRecord::GetRCIDasWstring() 
{
	return std::to_wstring(GetRCID());
}