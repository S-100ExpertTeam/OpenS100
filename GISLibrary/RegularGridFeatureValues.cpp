#include "stdafx.h"
#include "RegularGridFeatureValues.h"

using namespace S100HDF5;

RegularGridFeatureValues::RegularGridFeatureValues()
{
}


RegularGridFeatureValues::~RegularGridFeatureValues()
{
	for (auto i = _Values.begin(); i != _Values.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
}


void RegularGridFeatureValues::AddFloatValues(FloatValues* floatValues)
{
	_Values.push_back(floatValues);
}


FloatValues* RegularGridFeatureValues::GetFloatValues(int index)
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


int RegularGridFeatureValues::GetFloatValuesCount()
{
	return _Values.size();
}