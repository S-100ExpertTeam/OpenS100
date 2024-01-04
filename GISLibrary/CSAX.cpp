#include "stdafx.h"
#include "CSAX.h"

CSAX::CSAX()
{

}

CSAX::CSAX(const CSAX& other)
{
	m_axty = other.m_axty;
	m_axum = other.m_axum;
}

CSAX::CSAX(int axty, int axum)
{
	m_axty = axty;
	m_axum = axum;
}
CSAX::~CSAX()
{

}

CSAX CSAX::operator=(const CSAX& other)
{
	m_axty = other.m_axty;
	m_axum = other.m_axum;

	return *this;
}

int CSAX::GetSize() 
{
	return 2;
}