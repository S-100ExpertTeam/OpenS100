#include "stdafx.h"
#include "FloatValues.h"

using namespace S100HDF5;

FloatValues::FloatValues()
{

}


FloatValues::~FloatValues()
{
	delete[] _Values;
	_Values = nullptr;
}


std::string FloatValues::GetName()
{
	return _Name;
}


float* FloatValues::GetValues()
{
	return _Values;
}


int FloatValues::GetValueCount()
{
	return _ValueCount;
}


void FloatValues::SetName(std::string name)
{
	_Name = name;
}


void FloatValues::SetValues(float* values, int count)
{
	if (count > 0)
	{
		_Values = new float[count];
		memcpy_s(_Values, sizeof(float) * count, values, sizeof(float) * count);
		_ValueCount = count;
	}
	
}