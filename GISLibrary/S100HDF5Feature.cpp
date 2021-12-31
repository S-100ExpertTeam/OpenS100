#include "stdafx.h"
#include "S100HDF5Feature.h"

using namespace S100HDF5;

S100HDF5Feature::S100HDF5Feature()
{
}


S100HDF5Feature::~S100HDF5Feature()
{
	delete _TimeInformation;
	_TimeInformation = nullptr;
}


void S100HDF5Feature::SetDataCodingFormat(DataCodingFormat dataCodingFormat)
{
	_DataCodingFormat = dataCodingFormat;
}


void S100HDF5Feature::SetName(std::string name)
{
	_Name = name;
}


void S100HDF5Feature::SetTimeInformation(TimeInformation* timeInformation)
{
	if (!timeInformation)
	{
		return;
	}

	_TimeInformation = timeInformation;
}


DataCodingFormat S100HDF5Feature::GetDataCodingFormat()
{
	return _DataCodingFormat;
}


std::string S100HDF5Feature::GetName()
{
	return _Name;
}


TimeInformation* S100HDF5Feature::GetTimeInformation()
{
	return _TimeInformation;
}