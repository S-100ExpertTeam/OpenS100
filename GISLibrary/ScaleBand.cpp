#include "stdafx.h"
#include "ScaleBand.h"

ScaleBand::ScaleBand(int minDisplayScale, int optDisplayScale)
{
	this->minDisplayScale = minDisplayScale;
	this->optDisplayScale = optDisplayScale;
}

int ScaleBand::GetOptimumDisplayScale()
{
	return optDisplayScale;
}

void ScaleBand::SetOptimumDisplayScale(int value)
{
	optDisplayScale = value;
}

int ScaleBand::GetMinimumDisplayScale()
{
	return minDisplayScale;
}

void ScaleBand::SetMinimumDisplayScale(int value)
{
	minDisplayScale = value;
}