#include "stdafx.h"
#include "F_PROJ.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_PROJ::F_PROJ()
{
	m_prom = 0;
	m_prp1 = 0;
	m_prp2 = 0;
	m_prp3 = 0;
	m_prp4 = 0;
	m_prp5 = 0;
	m_feas = 0;
	m_fnor = 0;
}

F_PROJ::F_PROJ(const F_PROJ& other)
{
	m_prom = other.m_prom;
	m_prp1 = other.m_prp1;
	m_prp2 = other.m_prp2;
	m_prp3 = other.m_prp3;
	m_prp4 = other.m_prp4;
	m_prp5 = other.m_prp5;
	m_feas = other.m_feas;
	m_fnor = other.m_fnor;
}

F_PROJ::~F_PROJ()
{

}

F_PROJ F_PROJ::operator=(const F_PROJ& other)
{
	m_prom = other.m_prom;
	m_prp1 = other.m_prp1;
	m_prp2 = other.m_prp2;
	m_prp3 = other.m_prp3;
	m_prp4 = other.m_prp4;
	m_prp5 = other.m_prp5;
	m_feas = other.m_feas;
	m_fnor = other.m_fnor;

	return *this;
}

void F_PROJ::ReadField(BYTE *&buf)
{
	m_prom = *(buf++);
	m_prp1 = buf2double(buf, 8);
	m_prp2 = buf2double(buf, 8);
	m_prp3 = buf2double(buf, 8);
	m_prp4 = buf2double(buf, 8);
	m_prp5 = buf2double(buf, 8);
	m_feas = buf2double(buf, 8);
	m_fnor = buf2double(buf, 8);
}

void F_PROJ::ReadField(BYTE *&buf, int loopCnt)
{
	m_prom = *(buf++);
	m_prp1 = buf2double(buf, 8);
	m_prp2 = buf2double(buf, 8);
	m_prp3 = buf2double(buf, 8);
	m_prp4 = buf2double(buf, 8);
	m_prp5 = buf2double(buf, 8);
	m_feas = buf2double(buf, 8);
	m_fnor = buf2double(buf, 8);
}

int F_PROJ::GetFieldLength()
{
	int len = 0;
	len += F_PROJ::GetSize();
	return ++len;
}

int F_PROJ::GetSize()
{
	return 57;
}


