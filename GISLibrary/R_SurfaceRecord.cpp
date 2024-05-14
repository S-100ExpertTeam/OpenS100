#include "stdafx.h"
#include "R_SurfaceRecord.h"
#include "DRDirectoryInfo.h"
#include "F_INAS.h"
#include "F_RIAS.h"
#include "RIAS.h"

R_SurfaceRecord::R_SurfaceRecord(void)
{
	m_srid.m_name.RCNM = 130;
}

R_SurfaceRecord::R_SurfaceRecord(const R_SurfaceRecord& other) : R_VectorRecord(other)
{
	m_srid = other.m_srid;
	for (const auto& iter : other.m_rias)
	{
		F_RIAS* cont = new F_RIAS(*iter);
		m_rias.push_back(cont);
	}
}

R_SurfaceRecord::~R_SurfaceRecord(void)
{
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		delete* itor;
	}
	for (auto itor = m_rias.begin(); itor != m_rias.end(); itor++)
	{
		delete* itor;
	}
}

R_SurfaceRecord R_SurfaceRecord::operator=(const R_SurfaceRecord& other)
{
	for (auto& iter : m_rias)
	{
		if (iter)
		{
			delete iter;
			iter = nullptr;
		}
	}
	m_rias.clear();

	R_VectorRecord::operator=(other);

	m_srid = other.m_srid;
	for (const auto& iter : other.m_rias)
	{
		F_RIAS* cont = new F_RIAS(*iter);
		m_rias.push_back(cont);
	}

	return *this;
}

BOOL R_SurfaceRecord::ReadRecord(DRDirectoryInfo* dir, BYTE*& buf)
{
	USES_CONVERSION;
	for (int i = 0; i < dir->m_count; i++)
	{
		if (strcmp(dir->GetDirectory(i)->tag, "SRID") == 0)
		{
			m_srid.ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "INAS") == 0)
		{
			F_INAS* inas = new F_INAS();
			inas->ReadField(buf);
			m_inas.push_back(inas);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "RIAS") == 0)
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

bool R_SurfaceRecord::WriteRecord(CFile* file)
{
	directory.clear();

	// Set directory
	int fieldOffset = 0;
	int fieldLength = m_srid.GetFieldLength();
	Directory dir("SRID", fieldLength, fieldOffset);
	directory.push_back(dir);
	fieldOffset += fieldLength;

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		fieldLength = (*i)->GetFieldLength();
		Directory dir("INAS", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	for (auto i = m_rias.begin(); i != m_rias.end(); i++)
	{
		fieldLength = (*i)->GetFieldLength();
		Directory dir("RIAS", fieldLength, fieldOffset);
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
	m_srid.WriteField(file);

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		(*i)->WriteField(file);
	}

	for (auto i = m_rias.begin(); i != m_rias.end(); i++)
	{
		(*i)->WriteField(file);
	}

	return true;
}

RecordName R_SurfaceRecord::GetRecordName()
{
	return m_srid.m_name;
}

int R_SurfaceRecord::GetRCID()
{
	return m_srid.m_name.RCID;
}

std::string R_SurfaceRecord::GetRCIDasString(std::string prefix)
{
	return prefix + pugi::as_utf8(GetRCIDasWstring());
}

std::wstring R_SurfaceRecord::GetRCIDasWstring()
{
	return std::to_wstring(GetRCID());
}

void R_SurfaceRecord::SetRCID(int rcid)
{
	m_srid.m_name.RCID = rcid;
}

void R_SurfaceRecord::InsertRing(int rcnm, int rcid, int usag, int ornt)
{
	if (m_rias.size() == 0)
	{
		m_rias.push_back(new F_RIAS());
	}

	m_rias.front()->m_arr.push_back(new RIAS(rcnm, rcid, usag, ornt));
}

std::vector<RIAS*> R_SurfaceRecord::GetAllRIAS()
{
	std::vector<RIAS*> result;

	for (auto i = m_rias.begin(); i != m_rias.end(); i++) {
		auto rias = *i;
		for (auto j = rias->m_arr.begin(); j != rias->m_arr.end(); j++) {
			result.push_back(*j);
		}
	}

	return result;
}

