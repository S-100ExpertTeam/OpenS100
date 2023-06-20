#include "stdafx.h"
#include "IC2D.h"

IC2D::IC2D()
{

}

IC2D::IC2D(int m_ycoo, int m_xcoo)
{
	this->m_ycoo = m_ycoo;
	this->m_xcoo = m_xcoo;
}

IC2D::~IC2D()
{

}

int IC2D::GetSize()
{
	return 8;
}