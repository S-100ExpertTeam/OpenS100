#include "stdafx.h"
#include "ATTR.h"

ATTR::ATTR()
{

}

ATTR::ATTR(const ATTR& other)
{
	m_natc = other.m_natc;
	m_atix = other.m_atix;
	m_paix = other.m_paix;
	m_atin = other.m_atin;
	m_atvl = other.m_atvl;
}

ATTR::ATTR(int NATC, int ATIX, int PAIX, int ATIN, CString& ATVL)
{
	m_natc = NATC;
	m_atix = ATIX;
	m_paix = PAIX;
	m_atin = ATIN;
	m_atvl = ATVL;
}

ATTR::~ATTR()
{

}

int ATTR::GetOffsetToATVL()
{
	return 7;
}

std::string ATTR::getValueAsString()
{
	return pugi::as_utf8(m_atvl);
}