#include "stdafx.h"
#include "FC3D.h"

FC3D::FC3D()
{
	m_ycoo = 0.0;
	m_xcoo = 0.0;
	m_zcoo = 0.0;
}

FC3D::FC3D(const FC3D& other)
{
	m_ycoo = other.m_ycoo;
	m_xcoo = other.m_xcoo;
	m_zcoo = other.m_zcoo;
}

FC3D::FC3D(double ycoo, double xcoo, double zcoo)
{
	m_ycoo = ycoo;
	m_xcoo = xcoo;
	m_zcoo = zcoo;
}

FC3D::~FC3D()
{

}

FC3D FC3D::operator=(const FC3D& other)
{
	m_ycoo = other.m_ycoo;
	m_xcoo = other.m_xcoo;
	m_zcoo = other.m_zcoo;

	return *this;
}

int FC3D::GetSize()
{
	return 24;
}