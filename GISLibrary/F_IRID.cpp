#include "stdafx.h"
#include "F_IRID.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_IRID::F_IRID(void)
{

}

F_IRID::F_IRID(const F_IRID& other) : F_FRID(other)
{
	m_name = other.m_name;
	m_nftc = other.m_nftc;
	m_rver = other.m_rver;
	m_ruin = other.m_ruin;
}

F_IRID::F_IRID(RecordName recordName, int NFTC, int RVER, int RUIN)
{
	m_name = recordName;
	m_nftc = NFTC;
	m_rver = RVER;
	m_ruin = RUIN;
}

F_IRID::~F_IRID(void)
{

}

int F_IRID::NITC()
{
	return m_nftc;
}

void F_IRID::NITC(int value)
{
	m_nftc = value;
}