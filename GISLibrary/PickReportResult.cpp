#include "stdafx.h"
#include "PickReportResult.h"


using namespace S100HDF5;


PickReportResult::PickReportResult()
{
}


PickReportResult::~PickReportResult()
{
}


void PickReportResult::SetX(float x)
{
	_X = x;
}


void PickReportResult::SetY(float y)
{
	_Y = y;
}


void PickReportResult::AddValue(float value)
{
	_Values.push_back(value);
}


float PickReportResult::GetX()
{
	return _X;
}


float PickReportResult::GetY()
{
	return _Y;
}


float PickReportResult::GetValue(int index)
{
	try
	{
		return _Values.at(index);
	}
	catch (std::out_of_range)
	{
		return 0;
	}

	return 0;
}


void PickReportResult::SetTime(time_t time)
{
	_Time = time;
}


time_t PickReportResult::GetTime()
{
	return _Time;
}