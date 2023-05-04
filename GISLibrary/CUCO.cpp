#include "stdafx.h"
#include "CUCO.h"

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