#include "stdafx.h"
#include "ScaleBands.h"

ScaleBands::ScaleBands()
{

}

ScaleBands::ScaleBands(int minimumScale, int maximumScale)
{
	this->minimumScale = minimumScale;
	this->optimumScale = maximumScale;
}

ScaleBands::~ScaleBands()
{

}