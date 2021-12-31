#include "stdafx.h"
#include "FixedStationFeatureValues.h"


using namespace S100HDF5;


FixedStationFeatureValues::FixedStationFeatureValues()
{
}


FixedStationFeatureValues::~FixedStationFeatureValues()
{
	for (auto i = _Values.begin(); i != _Values.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
}


void FixedStationFeatureValues::SetName(std::string subregionName)
{
	_SubregionName = subregionName;
}


std::string FixedStationFeatureValues::GetName()
{
	return _SubregionName;
}


void FixedStationFeatureValues::AddFloatValues(FloatValues* floatValues)
{
	_Values.push_back(floatValues);
}


FloatValues* FixedStationFeatureValues::GetFloatValues(int index)
{
	try
	{
		return _Values.at(index);
	}
	catch (std::out_of_range)
	{
		return nullptr;
	}

	return nullptr;
}


int FixedStationFeatureValues::GetFloatValuesCount()
{
	return _Values.size();
}