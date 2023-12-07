#include "stdafx.h"
#include "R_CurveRecord.h"
#include "DRDirectoryInfo.h"
#include "F_PTAS.h"
#include "F_SEGH.h"
#include "F_SECC.h"
#include "F_COCC.h"
#include "F_C2IL.h"
#include "F_INAS.h"
#include "C2IL.h"
#include "PTAS.h"

#include <sstream>

R_CurveRecord::R_CurveRecord()
{
	m_crid.m_name.RCID = -1;
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

	if (m_segh)
	{
		delete m_segh;
		m_segh = nullptr;
	}

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
		if (strcmp(dir->GetDirectory(i)->tag, "CRID") == 0)
		{
			m_crid.ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "INAS") == 0)
		{
			F_INAS* inas = new F_INAS();
			inas->ReadField(buf);
			m_inas.push_back(inas);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "PTAS") == 0)
		{
			if (nullptr == m_ptas)
			{
				m_ptas = new F_PTAS();
			}

			auto cnt = (dir->GetDirectory(i)->length  - 1) / PTAS::GetSize();

			m_ptas->ReadField(buf, cnt);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "SECC") == 0)
		{
			if (nullptr == m_secc)
			{
				m_secc = new F_SECC();
			}

			m_secc->ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "SEGH") == 0)
		{
			m_segh = new F_SEGH();

			m_segh->ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "COCC") == 0)
		{
			if (nullptr == m_cocc)
			{
				m_cocc = new F_COCC();
			}

			m_cocc->ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "C2IL") == 0)
		{
			F_C2IL *c2il = new F_C2IL();

			auto cnt = (dir->GetDirectory(i)->length - 1) / C2IL::GetSize();

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

bool R_CurveRecord::WriteRecord(CFile* file)
{
	directory.clear();

	// Set directory
	int fieldOffset = 0;
	int fieldLength = m_crid.GetFieldLength();
	Directory dir("CRID", fieldLength, fieldOffset);
	directory.push_back(dir);
	fieldOffset += fieldLength;

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		fieldLength = (*i)->GetFieldLength();
		Directory dir("INAS", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	if (m_ptas)
	{
		fieldLength = m_ptas->GetFieldLength();
		Directory dir("PTAS", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	if (m_segh)
	{
		fieldLength = m_segh->GetFieldLength();
		Directory dir("SEGH", fieldLength, fieldOffset);
		directory.push_back(dir);
		fieldOffset += fieldLength;
	}

	for (auto i = m_c2il.begin(); i != m_c2il.end(); i++)
	{
		fieldLength = (*i)->GetFieldLength();
		Directory dir("C2IL", fieldLength, fieldOffset);
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
	m_crid.WriteField(file);

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		(*i)->WriteField(file);
	}

	if (m_ptas)
	{
		m_ptas->WriteField(file);
	}

	if (m_segh)
	{
		m_segh->WriteField(file);
	}

	for (auto i = m_c2il.begin(); i != m_c2il.end(); i++)
	{
		(*i)->WriteField(file);
	}

	return true;
}

RecordName R_CurveRecord::GetRecordName()
{
	return m_crid.m_name;
}

int R_CurveRecord::GetRCID() 
{
	return m_crid.m_name.RCID;
}

std::string R_CurveRecord::GetRCIDasString(std::string prefix)
{
	return prefix + pugi::as_utf8(GetRCIDasWstring());
}

std::wstring R_CurveRecord::GetRCIDasWstring() 
{
	return std::to_wstring(GetRCID());
}

void R_CurveRecord::SetRCID(int rcid)
{
	m_crid.m_name.RCID = rcid;
}

int R_CurveRecord::GetPointCount()
{
	int result = 0;

	auto countPTAS = m_ptas->m_arr.size();

	if (countPTAS != 1 && countPTAS != 2)
	{
		//OutputDebugString(L"Invalid count of PTAS of Curve Record\n");
		return 0;
	}

	// Separate start and last points
	result = 2;

	// C2IL
	if (m_c2il.size() == 1)
	{
		result += (int)m_c2il.front()->m_arr.size();
	}
	else
	{
		//OutputDebugString(L"Invalied C2IL count\n");
		return 0;
	}

	return result;
}

void R_CurveRecord::SetPTAS(int rcid)
{
	delete m_ptas;
	m_ptas = new F_PTAS();
	m_ptas->m_arr.push_back(new PTAS(rcid, 3));
}

void R_CurveRecord::SetPTAS(int begin_rcid, int end_rcid)
{
	delete m_ptas;
	m_ptas = new F_PTAS();
	m_ptas->m_arr.push_back(new PTAS(begin_rcid, 1));
	m_ptas->m_arr.push_back(new PTAS(end_rcid, 2));
}

void R_CurveRecord::AddPTAS(int rcid, int topi)
{
	if (!m_ptas)
		m_ptas = new F_PTAS();

	m_ptas->m_arr.push_back(new PTAS(rcid, topi));
}

void R_CurveRecord::SetSEGH(int intp, int circ, double ycoo, double xcoo, double dist, int disu, double sbrg, double angl)
{
	if (m_segh)
	{
		delete m_segh;
		m_segh = nullptr;
	}

	m_segh = new F_SEGH();

	m_segh->m_intp = intp;
	m_segh->m_circ = circ;
	m_segh->m_ycoo = ycoo;
	m_segh->m_xcoo = xcoo;
	m_segh->m_dist = dist;
	m_segh->m_disu = disu;
	m_segh->m_sbrg = sbrg;
	m_segh->m_angl = angl;
}

void R_CurveRecord::InsertC2IL(int x, int y)
{
	if (m_c2il.size() == 0)
	{
		m_c2il.push_back(new F_C2IL());
	}

	m_c2il.front()->m_arr.push_back(new C2IL(y, x));
}

void R_CurveRecord::InsertC2IL(int cmfx, int cmfy, SAFEARRAY* xcoo, SAFEARRAY* ycoo)
{
	if (xcoo == nullptr || xcoo->cDims != 1 ||
		ycoo == nullptr || ycoo->cDims != 1)
		return;

	if (m_c2il.size() == 0)
	{
		m_c2il.push_back(new F_C2IL());
	}

	LONG lBound, uBound;
	SafeArrayGetLBound(xcoo, 1, &lBound);
	SafeArrayGetUBound(xcoo, 1, &uBound);

	double* pxData = nullptr;
	SafeArrayAccessData(xcoo, reinterpret_cast<void**>(&pxData));
	double* pyData = nullptr;
	SafeArrayAccessData(ycoo, reinterpret_cast<void**>(&pyData));

	for (LONG i = lBound; i <= uBound; ++i)
	{
		double x = pxData[i] * cmfx;
		double y = pyData[i] * cmfy;

		m_c2il.front()->m_arr.push_back(new C2IL((int)y, (int)x));
	}

	SafeArrayUnaccessData(xcoo);
	SafeArrayUnaccessData(ycoo);

	return;
}

void R_CurveRecord::GetC2IL(double cmfx, double cmfy, SAFEARRAY** xcoo, SAFEARRAY** ycoo)
{
	if ((!xcoo) ||
		(!ycoo))
		return;

	CComSafeArray<double> xArray;
	CComSafeArray<double> yArray;

	xArray.Create();
	yArray.Create();

	int i = 0;
	for (const auto& f_c2il : m_c2il)
	{
		xArray.Resize(xArray.GetCount() + (int)f_c2il->m_arr.size());
		yArray.Resize(yArray.GetCount() + (int)f_c2il->m_arr.size());

		for (const auto& c2il : f_c2il->m_arr)
		{
			xArray.SetAt(i, static_cast<double>(c2il->m_xcoo / cmfx));
			yArray.SetAt(i, static_cast<double>(c2il->m_ycoo / cmfy));
			i++;
		}
	}

	*xcoo = xArray.Detach();
	*ycoo = yArray.Detach();

	return;
}

int R_CurveRecord::GetBeginningPointRCID()
{
	if (m_ptas)
	{
		for (auto i = m_ptas->m_arr.begin(); i != m_ptas->m_arr.end(); i++)
		{
			if ((*i)->m_topi == 1 || (*i)->m_topi == 3)
			{
				return (*i)->m_name.RCID;
			}
		}
	}

	return -1;
}

int R_CurveRecord::GetEndPointRCID()
{
	if (m_ptas)
	{
		for (auto i = m_ptas->m_arr.begin(); i != m_ptas->m_arr.end(); i++)
		{
			if ((*i)->m_topi == 2 || (*i)->m_topi == 3)
			{
				return (*i)->m_name.RCID;
			}
		}
	}

	return -1;
}

std::string R_CurveRecord::GetBeginningPointRCIDasString(std::string prefix)
{
	if (m_ptas)
	{
		for (auto i = m_ptas->m_arr.begin(); i != m_ptas->m_arr.end(); i++)
		{
			if ((*i)->m_topi == 1 || (*i)->m_topi == 3)
			{
				return (*i)->m_name.GetRCIDasString(prefix);
			}
		}
	}
	
	return "";
}

std::string R_CurveRecord::GetEndPointRCIDasString(std::string prefix)
{
	if (m_ptas)
	{
		for (auto i = m_ptas->m_arr.begin(); i != m_ptas->m_arr.end(); i++)
		{
			if ((*i)->m_topi == 2 || (*i)->m_topi == 3)
			{
				return (*i)->m_name.GetRCIDasString(prefix);
			}
		}
	}

	return "";
}

std::string R_CurveRecord::GetC2ILString(int CMFX, int CMFY)
{
	std::stringstream ss;
	ss.precision(7);

	if (m_c2il.size() == 1)
	{
		for (auto i = m_c2il.front()->m_arr.begin(); i != m_c2il.front()->m_arr.end(); i++)
		{
			if (i != m_c2il.front()->m_arr.begin())
			{
				ss << " ";
			}

			ss << (*i)->m_ycoo / CMFY << " " << (*i)->m_xcoo / CMFX;
		}
	}

	return ss.str();
}

std::vector<C2IL*> R_CurveRecord::GetAllC2IL()
{
	std::vector<C2IL*> result;

	for (auto i = m_c2il.begin(); i != m_c2il.end(); i++) {
		auto c2il = *i;
		for (auto j = c2il->m_arr.begin(); j != c2il->m_arr.end(); j++) {
			result.push_back(*j);
		}
	}

	return result;
}