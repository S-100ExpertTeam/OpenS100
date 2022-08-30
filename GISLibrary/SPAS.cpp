#include "stdafx.h"
#include "SPAS.h"

int SPAS::GetSize()
{
	return 15;
}

void SPAS::Set(RecordName recordName, int ORNT, int SMIN, int SMAX, int SAUI)
{
	m_name = recordName;
	m_ornt = ORNT;
	m_smin = SMIN;
	m_smax = SMAX;
	m_saui = SAUI;
}