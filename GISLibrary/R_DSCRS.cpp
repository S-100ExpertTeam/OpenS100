#include "stdafx.h"
#include "R_DSCRS.h"
#include "DRDirectoryInfo.h"
#include "F_VDAT.h"
#include "F_CRSH.h"
#include "F_CSAX.h"
#include "F_PROJ.h"
#include "F_GDAT.h"
#include "CSAX.h"

R_DSCRS::R_DSCRS()
{
	Init();
}

R_DSCRS::~R_DSCRS()
{
	Release();
}

#pragma warning(disable:4244)
#pragma warning(disable:4018)
BOOL R_DSCRS::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	Release();

	USES_CONVERSION;
	for(unsigned i = 0; i < dir->m_count; i++)
	{
		if (strcmp(dir->GetDirectory(i)->tag, "CSID") == 0)
		{
			m_csid.ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "CRSH") == 0)
		{
			F_CRSH *crsh = new F_CRSH();
			crsh->ReadField(buf);
			m_crsh.push_back(crsh);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "CSAX") == 0)
		{
			if (!m_csax) m_csax = new F_CSAX();
			m_csax->ReadField(buf);
		}
		else if (strcmp(dir->GetDirectory(i)->tag, "VDAT") == 0)
		{
			if (!m_vdat) m_vdat = new F_VDAT();
			m_vdat->ReadField(buf);
		}
		else
		{
			buf += dir->GetDirectory(i)->length;
		}

		if (*(buf++) != 0x1E)
		{
			OutputDebugString(L"terminator error\n");
		}
	}

	return TRUE;
}

bool R_DSCRS::WriteRecord(CFile* file)
{
	directory.clear();

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

void R_DSCRS::Init()
{
	auto crsh_horizontal = new F_CRSH();
	crsh_horizontal->m_crix = 1;
	crsh_horizontal->m_crst = 1;
	crsh_horizontal->m_csty = 1;
	crsh_horizontal->m_crnm = L"WGS84";
	crsh_horizontal->m_crsi = L"4326";
	crsh_horizontal->m_crss = 2;
	crsh_horizontal->m_scri = L"";
	m_crsh.push_back(crsh_horizontal);

	auto crsh_vertical = new F_CRSH();
	crsh_vertical->m_crix = 2;
	crsh_vertical->m_crst = 5;
	crsh_vertical->m_csty = 3;
	crsh_vertical->m_crnm = L"meanHighWaterSprings"; 
	crsh_vertical->m_crsi = L"";
	crsh_vertical->m_crss = 255;
	crsh_vertical->m_scri = L"";
	m_crsh.push_back(crsh_vertical);

	m_csax = new F_CSAX();
	auto csax = new CSAX();
	csax->m_axty = 12;
	csax->m_axum = 4;
	m_csax->m_arr.push_back(csax);
	
	m_vdat = new F_VDAT();
	m_vdat->m_dtnm = L"meanHighWaterSprings";
	m_vdat->m_dtid = L"17";
	m_vdat->m_dtsr = 2;
	m_vdat->m_scri = L"";
}

void R_DSCRS::Release()
{
	for (auto itor = m_crsh.begin(); itor != m_crsh.end(); itor++)
	{
		delete* itor;
	}
	m_crsh.clear();

	delete m_csax;
	m_csax = nullptr;

	//delete m_proj;
	//m_proj = nullptr;

	//delete m_gdat;
	//m_gdat = nullptr;

	delete m_vdat;
	m_vdat = nullptr;
}