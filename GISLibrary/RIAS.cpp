#include "stdafx.h"
#include "RIAS.h"

RIAS::RIAS()
{

}

RIAS::RIAS(const RIAS& other)
{
	m_name = other.m_name;
	m_usag = other.m_usag;
	m_ornt = other.m_ornt;
}

RIAS::RIAS(int rcnm, int rcid, int usag, int ornt)
{
	m_name.RCNM = rcnm;
	m_name.RCID = rcid;
	m_usag = usag;
	m_ornt = ornt;
}

RIAS::~RIAS()
{

}

RIAS RIAS::operator=(const RIAS& other)
{
	m_name = other.m_name;
	m_usag = other.m_usag;
	m_ornt = other.m_ornt;

	return *this;
}

int RIAS::GetSize()
{
	return 8;
}

bool RIAS::IsForward()
{
	return m_ornt == 1;
}

bool RIAS::IsReverse()
{
	return m_ornt == 2;
}

bool RIAS::IsExterior()
{
	return m_usag == 1;
}

bool RIAS::IsInterior()
{
	return m_usag == 2;
}

bool RIAS::IsCurve()
{
	return m_name.RCNM == 120;
}

bool RIAS::IsCompositeCurve()
{
	return m_name.RCNM == 125;
}