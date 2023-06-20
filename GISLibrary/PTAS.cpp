#include "stdafx.h"
#include "PTAS.h"

PTAS::PTAS()
{
	m_name.RCNM = 110;
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

int PTAS::GetSize()
{
	return 6;
}