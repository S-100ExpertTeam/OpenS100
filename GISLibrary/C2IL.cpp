#include "stdafx.h"
#include "C2IL.h"

C2IL::C2IL()
{

}

C2IL::C2IL(int m_ycoo, int m_xcoo)
{
	this->m_ycoo = m_ycoo;
	this->m_xcoo = m_xcoo;
}

C2IL::~C2IL()
{

}

int C2IL::GetSize()
{
	return 8;
}