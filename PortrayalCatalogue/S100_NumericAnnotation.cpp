#include "stdafx.h"
#include "S100_NumericAnnotation.h"

S100_NumericAnnotation::S100_NumericAnnotation()
{

}

S100_NumericAnnotation::~S100_NumericAnnotation()
{

}

void S100_NumericAnnotation::setDecimals(int value)
{
	decimals = value;
}

int S100_NumericAnnotation::getDecimals()
{
	return decimals;
}

void S100_NumericAnnotation::setBodySize(double value)
{
	bodySize = value;
}

double S100_NumericAnnotation::getBodySize()
{
	return bodySize;
}

void S100_NumericAnnotation::setBuffer(double value)
{
	buffer = value;
}

std::optional<double> S100_NumericAnnotation::getBuffer()
{
	return buffer;
}

void S100_NumericAnnotation::setChampion(Enum_ChampionChoice value)
{
	champion = value;
}

Enum_ChampionChoice S100_NumericAnnotation::getChampion()
{
	return champion;
}
