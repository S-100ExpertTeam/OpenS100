#include "stdafx.h"
#include "SENC_VectorPoint.h"

SENC_VectorPoint::SENC_VectorPoint()
{

}

SENC_VectorPoint::~SENC_VectorPoint()
{

}

double SENC_VectorPoint::GetX()
{
	return x;
}

void SENC_VectorPoint::SetX(double _x)
{
	this->x = _x;
}

double SENC_VectorPoint::GetY()
{
	return y;
}

void SENC_VectorPoint::SetY(double _y)
{
	this->y = _y;
}