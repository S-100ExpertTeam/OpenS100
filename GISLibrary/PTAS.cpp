#include "stdafx.h"
#include "PTAS.h"

PTAS::PTAS()
{
	m_name.RCNM = 110;
}

PTAS::PTAS(const PTAS& other)
{
	m_name = other.m_name;
	m_topi = other.m_topi;
}

PTAS::PTAS(int rcid, int topi)
{
	m_name.RCNM = 110;
	m_name.RCID = rcid;
	m_topi = topi;
}

PTAS::~PTAS()
{

}

PTAS PTAS::operator=(const PTAS& other)
{
	m_name = other.m_name;
	m_topi = other.m_topi;

	return *this;
}

int PTAS::GetSize()
{
	return 6;
}