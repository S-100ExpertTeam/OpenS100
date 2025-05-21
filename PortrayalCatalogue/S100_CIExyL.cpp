#include "stdafx.h"
#include "S100_CIExyL.h"

S100_CIExyL::S100_CIExyL(double x, double y, double L)
{
	setX(x);
	setY(y);
	setL(L);
}

S100_CIExyL::~S100_CIExyL()
{

}

void S100_CIExyL::setX(double value)
{
	x = value;
}

void S100_CIExyL::setY(double value)
{
	y = value;
}

void S100_CIExyL::setL(double value)
{
	L = value;
}

double S100_CIExyL::getX()
{
	return x;
}

double S100_CIExyL::getY()
{
	return y;
}

double S100_CIExyL::getL()
{
	return L;
}
