#include "stdafx.h"
#include "MASK.h"

MASK::MASK()
{

}

MASK::MASK(const MASK& other)
{
	m_name = other.m_name;
	m_mind = other.m_mind;
	m_muin = other.m_muin;
}

MASK::MASK(RecordName name, int mind, int muin)
{
	m_name = name;
	m_mind = mind;
	m_muin = muin;
}

MASK::~MASK()
{

}

MASK MASK::operator=(const MASK& other)
{
	m_name = other.m_name;
	m_mind = other.m_mind;
	m_muin = other.m_muin;

	return *this;
}

int MASK::GetSize()
{
	return 7;
}
