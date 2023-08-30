#include "stdafx.h"
#include "DataCoverage.h"

DataCoverage::DataCoverage()
{

}

DataCoverage::~DataCoverage()
{

}

void DataCoverage::setMinDS(int value)
{
	minDS = value;
}

int DataCoverage::getMinDS()
{
	return minDS;
}

void DataCoverage::setMaxDS(int value)
{
	maxDS = value;
}

int DataCoverage::getMaxDS()
{
	return maxDS;
}