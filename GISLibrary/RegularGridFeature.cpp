#include "stdafx.h"
#include "RegularGridFeature.h"

using namespace S100HDF5;

RegularGridFeature::RegularGridFeature()
{
}


RegularGridFeature::~RegularGridFeature()
{
	delete _RegularGridInformation;
	_RegularGridInformation = nullptr;

	for (auto i = _Values.begin(); i != _Values.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
}


void RegularGridFeature::SetRegularGridInformation(RegularGridInformation* regularGridInformation)
{
	_RegularGridInformation = regularGridInformation;
}


RegularGridInformation* RegularGridFeature::GetRegularGridInformation()
{
	return _RegularGridInformation;
}


void RegularGridFeature::AddRegularGridFeatureValues(RegularGridFeatureValues* featureValues)
{
	_Values.push_back(featureValues);
}


RegularGridFeatureValues* RegularGridFeature::GetRegularGridFeatureValues(int index)
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


RegularGridFeatureValues* RegularGridFeature::GetNearestRegularGridFeatureValues(time_t time, TimeInformation* timeInformation)
{
	int findIndex = timeInformation->GetNearestTimeIndex(time);
	return GetRegularGridFeatureValues(findIndex);
}