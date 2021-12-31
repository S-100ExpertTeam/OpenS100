#include "stdafx.h"
#include "FixedStationFeature.h"


using namespace S100HDF5;


FixedStationFeature::FixedStationFeature()
{
}


FixedStationFeature::~FixedStationFeature()
{
	delete _FixedStationInformation;
	_FixedStationInformation = nullptr;

	for (auto i = _Values.begin(); i != _Values.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
}


void FixedStationFeature::SetFixedStationInformation(FixedStationInformation* fixedStationInformation)
{
	_FixedStationInformation = fixedStationInformation;
}


FixedStationInformation* FixedStationFeature::GetFixedStationInformation()
{
	return _FixedStationInformation;
}


void FixedStationFeature::AddFixedStationFeatureValues(FixedStationFeatureValues* value)
{
	_Values.push_back(value);
}


FixedStationFeatureValues* FixedStationFeature::GetFixedStationFeatureValues(int index)
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


FixedStationFeatureValues* FixedStationFeature::GetNearestIrregularGridFeatureValues(time_t time, TimeInformation* timeInformation)
{
	int nearestIndex = timeInformation->GetNearestTimeIndex(time);
	return GetFixedStationFeatureValues(nearestIndex);
}