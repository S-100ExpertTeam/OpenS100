#include "stdafx.h"
#include "ATTR.h"

ATTR::ATTR()
{

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