#include "stdafx.h"
#include "SPAS.h"

SPAS::SPAS()
{

}

SPAS::SPAS(const SPAS& other)
{
	m_name = other.m_name;
	m_ornt = other.m_ornt;
	m_smin = other.m_smin;
	m_smax = other.m_smax;
	m_saui = other.m_saui;
}

SPAS::SPAS(RecordName name, int ornt, UINT smin, UINT smax, int saui)
{
	m_name = name;
	m_ornt = ornt;
	m_smin = smin;
	m_smax = smax;
	m_saui = saui;
}

SPAS::~SPAS()
{

}

SPAS SPAS::operator=(const SPAS& other)
{
	m_name = other.m_name;
	m_ornt = other.m_ornt;
	m_smin = other.m_smin;
	m_smax = other.m_smax;
	m_saui = other.m_saui;

	return *this;
}

int SPAS::GetSize()
{
	return 15;
}

void SPAS::Set(RecordName recordName, int ORNT, unsigned int SMIN, unsigned int SMAX, int SAUI)
{
	m_name = recordName;

	if (115 == m_name.RCNM)
	{
		m_ornt = 255;
	}
	else
	{
		m_ornt = ORNT;
	}

	m_smin = SMIN;
	m_smax = SMAX;
	m_saui = SAUI;
}