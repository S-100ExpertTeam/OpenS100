#include "stdafx.h"
#include "THAS.h"

THAS::THAS()
{

}

THAS::THAS(const THAS& other)
{
	m_name = other.m_name;
	m_taui = other.m_taui;
}

THAS::THAS(RecordName name, int taui)
{
	m_name = name;
	m_taui = taui;
}

THAS::~THAS()
{

}

THAS THAS::operator=(const THAS& other)
{
	m_name = other.m_name;
	m_taui = other.m_taui;

	return *this;
}

int THAS::GetSize()
{
	return 6;
}