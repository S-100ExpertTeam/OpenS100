#include "stdafx.h"
#include "GraphicBasePackage.h"

GraphicBasePackage::Vector::Vector()
{

}

GraphicBasePackage::Vector::Vector(double x, double y)
{
	this->x = x;
	this->y = y;
}

GraphicBasePackage::Vector::~Vector()
{

}

double GraphicBasePackage::Vector::GetX() const
{
	return x;
}

void GraphicBasePackage::Vector::SetX(double value)
{
	this->x = value;
}

double GraphicBasePackage::Vector::GetY() const
{
	return y;
}

void GraphicBasePackage::Vector::SetY(double value)
{
	this->y = value;
}

void GraphicBasePackage::Vector::Set(double x, double y)
{
	SetX(x);
	SetY(y);
}