#include "stdafx.h"
#include "CUCO.h"

CUCO::CUCO()
{

}

CUCO::CUCO(const CUCO& other)
{
	m_name = other.m_name;
	m_ornt = other.m_ornt;
}

CUCO::CUCO(int rcnm, int rcid, int ornt)
{
	m_name.RCNM = rcnm;
	m_name.RCID = rcid;
	m_ornt = ornt;
}

CUCO::~CUCO()
{

}

CUCO CUCO::operator=(const CUCO& other)
{
	m_name = other.m_name;
	m_ornt = other.m_ornt;

	return *this;
}

int CUCO::GetSize() 
{
	return 6;
}

bool CUCO::IsCurve()
{
	return m_name.RCNM == 120;
}

bool CUCO::IsCompositeCurve()
{
	return m_name.RCNM == 125;
}