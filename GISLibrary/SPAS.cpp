#include "stdafx.h"
#include "SPAS.h"

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