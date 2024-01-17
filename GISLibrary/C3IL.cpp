#include "stdafx.h"
#include "C3IL.h"

C3IL::C3IL()
{

}

C3IL::C3IL(const C3IL& other)
{
	m_ycoo = other.m_ycoo;
	m_xcoo = other.m_xcoo;
	m_zcoo = other.m_zcoo;
}

C3IL::C3IL(int x, int y, int z)
{
	m_ycoo = y;
	m_xcoo = x;
	m_zcoo = z;
}

C3IL::~C3IL()
{

}

C3IL C3IL::operator=(const C3IL& other)
{
	m_ycoo = other.m_ycoo;
	m_xcoo = other.m_xcoo;
	m_zcoo = other.m_zcoo;

	return *this;
}

int C3IL::GetSize()
{
	return 12;
}