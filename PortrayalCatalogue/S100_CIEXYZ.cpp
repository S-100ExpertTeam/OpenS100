#include "stdafx.h"
#include "S100_CIEXYZ.h"

S100_CIEXYZ::S100_CIEXYZ()
{

}

S100_CIEXYZ::S100_CIEXYZ(double X, double Y, double Z)
{
	setX(X);
	setY(Y);
	setZ(Z);
}

S100_CIEXYZ::~S100_CIEXYZ()
{

}

void S100_CIEXYZ::setX(double value)
{
	X = value;
}

void S100_CIEXYZ::setY(double value)
{
	Y = value;
}

void S100_CIEXYZ::setZ(double value)
{
	Z = value;
}

double S100_CIEXYZ::getX()
{
	return X;
}

double S100_CIEXYZ::getY()
{
	return Y;
}

double S100_CIEXYZ::getZ()
{
	return Z;
}
