#include "stdafx.h"
#include "C2IL.h"

C2IL::C2IL()
{

}

C2IL::C2IL(const C2IL& other)
{
	m_ycoo = other.m_ycoo;
	m_xcoo = other.m_xcoo;
}

C2IL::C2IL(int m_ycoo, int m_xcoo)
{
	this->m_ycoo = m_ycoo;
	this->m_xcoo = m_xcoo;
}

C2IL::~C2IL()
{

}

C2IL C2IL::operator=(const C2IL& other)
{
	m_ycoo = other.m_ycoo;
	m_xcoo = other.m_xcoo;

	return *this;
}

int C2IL::GetSize()
{
	return 8;
}