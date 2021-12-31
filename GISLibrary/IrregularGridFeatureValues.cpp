#include "stdafx.h"
#include "IrregularGridFeatureValues.h"


using namespace S100HDF5;


char* IrregularGridFeatureValues::TimePointAttributeName = "timePoint";


IrregularGridFeatureValues::IrregularGridFeatureValues()
{
}


IrregularGridFeatureValues::~IrregularGridFeatureValues()
{
	for (auto i = _Values.begin(); i != _Values.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
}


void IrregularGridFeatureValues::SetTime(time_t TimePoint)
{
	_TimePoint = TimePoint;
}


void IrregularGridFeatureValues::AddFloatValues(FloatValues* Values)
{
	_Values.push_back(Values);
}


time_t IrregularGridFeatureValues::GetTime()
{
	return _TimePoint;
}


FloatValues* IrregularGridFeatureValues::GetFloatValues(int index)
{
	try
	{ 
		return _Values.at(index);
	}
	catch (std::out_of_range e)
	{
		return nullptr;
	}

	return nullptr;
}


int IrregularGridFeatureValues::GetFloatValuesCount()
{
	return _Values.size();
}