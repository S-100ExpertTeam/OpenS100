#include "stdafx.h"
#include "R_PointRecord.h"
#include "DRDirectoryInfo.h"
#include "F_C2IT.h"
#include "F_C3IT.h"
#include "F_INAS.h"
#include "SPoint.h"

R_PointRecord::R_PointRecord(void)
{
	m_prid.m_name.RCID = -1;
	m_prid.m_name.RCNM = 110;
}

R_PointRecord::R_PointRecord(const R_PointRecord& other) : R_VectorRecord(other)
{
	m_prid = other.m_prid;
	if (other.m_c2it)
		m_c2it = new F_C2IT(*other.m_c2it);
	if (other.m_c3it)
		m_c3it = new F_C3IT(*other.m_c3it);
}

R_PointRecord::~R_PointRecord(void)
{
	delete m_c2it;
	delete m_c3it;
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		delete* itor;
	}
}

R_PointRecord R_PointRecord::operator=(const R_PointRecord& other)
{
	if (m_c2it)
	{
		delete m_c2it;
		m_c2it = nullptr;
	}

	if (m_c3it)
	{
		delete m_c3it;
		m_c3it = nullptr;
	}

	R_VectorRecord::operator=(other);

	m_prid = other.m_prid;
	if (other.m_c2it)
		m_c2it = new F_C2IT(*other.m_c2it);
	if (other.m_c3it)
		m_c3it = new F_C3IT(*other.m_c3it);

	return *this;
}

BOOL R_PointRecord::ReadRecord(DRDirectoryInfo* dir, BYTE*& buf)
{
	USES_CONVERSION;
	int i = 0, j = 0;
	for (i = 0; i < dir->m_count; i++)
	{
		if (strcmp(dir->GetDirectory(i)->tag, "PRID") == 0)
		{
			m_prid.ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "INAS") == 0)
		{
			F_INAS* inas = new F_INAS();
			inas->ReadField(buf);
			m_inas.push_back(inas);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "C2IT") == 0)
		{
			m_c2it = new F_C2IT();
			m_c2it->ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "C3IT") == 0)
		{
			m_c3it = new F_C3IT();
			m_c3it->ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "C2FT") == 0)
		{
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "C3FT") == 0)
		{
		}
		else
		{
			buf += dir->GetDirectory(i)->length;
		}

		if (*(buf++) != 0x1E)//{}
			TRACE(W2A(TEXT("terminator error")));
	}

	return TRUE;
}

bool R_PointRecord::WriteRecord(CFile* file)
{
	directory.clear();

	// Set directory
	int fieldOffset = 0;
	int fieldLength = m_prid.GetFieldLength();
	Directory dir("PRID", fieldLength, fieldOffset);
	directory.push_back(dir);
	fieldOffset += fieldLength;

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		fieldLength = (*i)->GetFieldLength();
		Directory dir("INAS", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	if (m_c2it)
	{
		fieldLength = m_c2it->GetFieldLength();
		Directory dir("C2IT", fieldLength, fieldOffset);
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
	m_prid.WriteField(file);

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		(*i)->WriteField(file);
	}

	if (m_c2it)
	{
		m_c2it->WriteField(file);
	}

	return true;
}

RecordName R_PointRecord::GetRecordName()
{
	return m_prid.m_name;
}

int R_PointRecord::GetRCID()
{
	return m_prid.m_name.RCID;
}

std::string R_PointRecord::GetRCIDasString(std::string prefix)
{
	return prefix + pugi::as_utf8(GetRCIDasWstring());
}

std::wstring R_PointRecord::GetRCIDasWstring()
{
	return std::to_wstring(GetRCID());
}

void R_PointRecord::SetRCID(int rcid)
{
	m_prid.m_name.RCID = rcid;
}

void R_PointRecord::SetC2IT(F_C2IT* value)
{
	m_c2it = value;
}

void R_PointRecord::SetC2IT(int x, int y)
{
	if (m_c3it)
	{
		delete m_c3it;
		m_c3it = nullptr;
	}

	if (m_c2it == nullptr)
	{
		m_c2it = new F_C2IT();
	}

	m_c2it->m_xcoo = x;
	m_c2it->m_ycoo = y;
}

void R_PointRecord::SetC3IT(F_C3IT* value)
{
	m_c3it = value;
}

void R_PointRecord::SetC3IT(int x, int y, int z)
{
	if (m_c2it)
	{
		delete m_c2it;
		m_c2it = nullptr;
	}

	if (m_c3it == nullptr)
	{
		m_c3it = new F_C3IT();
	}

	m_c3it->m_xcoo = x;
	m_c3it->m_ycoo = y;
	m_c3it->m_zcoo = z;
}

