#include "stdafx.h"
#include "FixedStationInformation.h"

using namespace S100HDF5;

FixedStationInformation::FixedStationInformation()
{
}


FixedStationInformation::~FixedStationInformation()
{
	delete[] _Positioning;
	_Positioning = nullptr;
}


int FixedStationInformation::GetNumberOfTimes()
{
	return _NumberOfTimes;
}


void FixedStationInformation::SetNumberOfTimes(int num)
{
	_NumberOfTimes = num;
}


void FixedStationInformation::SetPositioning(S100HDF5::Point* positioning, int numberOfNodes)
{
	_Positioning = positioning;
	_NumberOfNode = numberOfNodes;
}


S100HDF5::Point* FixedStationInformation::GetPositioning()
{
	return _Positioning;
}


int FixedStationInformation::GetNumberOfNode()
{
	return _NumberOfNode;
}