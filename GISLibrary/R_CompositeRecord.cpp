#include "stdafx.h"
#include "R_CompositeRecord.h"
#include "DRDirectoryInfo.h"
#include "F_CCOC.h"
#include "F_CUCO.h"
#include "F_INAS.h"
#include "CUCO.h"

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
		if (strcmp(dir->GetDirectory(i)->tag, "CCID") == 0)
		{
			m_ccid.ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "INAS") == 0)
		{
			F_INAS* inas = new F_INAS();

			inas->ReadField(buf);

			m_inas.push_back(inas);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "COCC") == 0)
		{
			if (nullptr == m_ccoc)
			{
				m_ccoc = new F_CCOC();
			}

			m_ccoc->ReadField(buf);
		}
		else if(strcmp(dir->GetDirectory(i)->tag, "CUCO") == 0)
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

bool R_CompositeRecord::WriteRecord(CFile* file)
{
	directory.clear();

	// Set directory
	int fieldOffset = 0;
	int fieldLength = m_ccid.GetFieldLength();
	Directory dir("CCID", fieldLength, fieldOffset);
	directory.push_back(dir);
	fieldOffset += fieldLength;

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		fieldLength = (*i)->GetFieldLength();
		Directory dir("INAS", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	for (auto i = m_cuco.begin(); i != m_cuco.end(); i++)
	{
		fieldLength = (*i)->GetFieldLength();
		Directory dir("CUCO", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	int totalFieldSize = fieldOffset;

	// Set leader
	SetLeader(totalFieldSize, false);
	leader.SetAsDR();
	leader.WriteLeader(file);

	// Write directory
	WriteDirectory(file);

	// Write field area
	m_ccid.WriteField(file);

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		(*i)->WriteField(file);
	}

	for (auto i = m_cuco.begin(); i != m_cuco.end(); i++)
	{
		(*i)->WriteField(file);
	}

	return true;
}

RecordName R_CompositeRecord::GetRecordName()
{
	return m_ccid.m_name;
}

int R_CompositeRecord::GetRCID()
{
	return m_ccid.m_name.RCID;
}

std::string R_CompositeRecord::GetRCIDasString(std::string prefix)
{
	return prefix + pugi::as_utf8(GetRCIDasWstring());
}

std::wstring R_CompositeRecord::GetRCIDasWstring() 
{
	return std::to_wstring(GetRCID());
}

void R_CompositeRecord::SetRCID(int rcid)
{
	m_ccid.m_name.RCID = rcid;
}

void R_CompositeRecord::InsertCurve(int rcnm, int rcid, int ornt)
{
	if (m_cuco.size() == 0)
	{
		m_cuco.push_back(new F_CUCO);
	}

	m_cuco.front()->m_arr.push_back(new CUCO(rcnm, rcid, ornt));
}