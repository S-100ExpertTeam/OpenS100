#include "stdafx.h"
#include "FC2D.h"

FC2D::FC2D()
{

}

FC2D::FC2D(const FC2D& other)
{
	m_ycoo = other.m_ycoo;
	m_xcoo = other.m_xcoo;
}

FC2D::FC2D(double ycoo, double xcoo)
{
	m_ycoo = ycoo;
	m_xcoo = xcoo;
}

FC2D::~FC2D()
{

}

FC2D FC2D::operator=(const FC2D& other)
{
	m_ycoo = other.m_ycoo;
	m_xcoo = other.m_xcoo;

	return *this;
}

int FC2D::GetSize()
{
	return 16;
}