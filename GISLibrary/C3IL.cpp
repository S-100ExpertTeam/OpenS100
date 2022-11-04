#include "stdafx.h"
#include "C3IL.h"

C3IL::C3IL()
{

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

int C3IL::GetSize()
{
	return 12;
}