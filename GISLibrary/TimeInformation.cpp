#include "stdafx.h"
#include "TimeInformation.h"


using namespace S100HDF5;


char* TimeInformation::DateTimeOfFirstRecordAttributeName = "dateTimeOfFirstRecord";
char* TimeInformation::DateTimeOfLastRecordAttributeName = "dateTimeOfLastRecord";
char* TimeInformation::TimeRecordIntervalAttributeName = "timeRecordInterval";


TimeInformation::TimeInformation()
{
}


TimeInformation::~TimeInformation()
{

}


void TimeInformation::SetTimeInformation(
	time_t DateTimeOfFirstRecord,
	time_t DateTimeOfLastRecord,
	int TimeRecordInterval)
{
	_DateTimeOfFirstRecord = DateTimeOfFirstRecord;
	_DateTimeOfLastRecord = DateTimeOfLastRecord;
	_TimeRecordInterval = TimeRecordInterval;

	__int64 period = _DateTimeOfLastRecord - _DateTimeOfFirstRecord;
	if (period == 0)
	{
		_NumberOfTimes = 1;
	}
	else if (period > 0)
	{
		if (_TimeRecordInterval > 0)
		{
			_NumberOfTimes = (period / _TimeRecordInterval) + 1;
		}
		else
		{
			_NumberOfTimes = 0;
		}
	}
	else if (period < 0)
	{
		_NumberOfTimes = 0;
	}
}

int TimeInformation::GetNumberOfTimes()
{
	return _NumberOfTimes;
}


int TimeInformation::GetTimeRecordInterval()
{
	return _TimeRecordInterval;
}


time_t TimeInformation::GetDateTimeOfFirstRecord()
{
	return _DateTimeOfFirstRecord;
}


time_t TimeInformation::GetDateTimeOfLastRecord()
{
	return _DateTimeOfLastRecord;
}


time_t TimeInformation::GetTimeTfromISO8601Basic(std::string text)
{
	// text ex) 20190621T060000Z

	tm _tm = { 0, };
	std::string strYear = text.substr(0, 4);
	std::string strMon = text.substr(4, 2);
	std::string strDay = text.substr(6, 2);
	std::string strHour = text.substr(9, 2);
	std::string strMin = text.substr(11, 2);
	std::string strSec = text.substr(13, 2);

	_tm.tm_year = atoi(strYear.c_str()) - 1900;
	_tm.tm_mon = atoi(strMon.c_str()) - 1;
	_tm.tm_mday = atoi(strDay.c_str());
	_tm.tm_hour = atoi(strHour.c_str());
	_tm.tm_min = atoi(strMin.c_str());
	_tm.tm_sec = atoi(strSec.c_str());

	return mktime(&_tm);
}


int TimeInformation::GetNearestTimeIndex(time_t time)
{
	if (time < _DateTimeOfFirstRecord)
	{
		return 0;
	}
	else if (time > _DateTimeOfLastRecord)
	{
		return _NumberOfTimes - 1;
	}

	int findIndex = 0;

	__int64 offset = time - _DateTimeOfFirstRecord;
	__int64 d = offset % _TimeRecordInterval;
	findIndex = offset / _TimeRecordInterval;

	if (d > (_TimeRecordInterval / 2))
	{
		findIndex + 1;
	}

	return findIndex;
}


time_t TimeInformation::GetTime(int index)
{
	return _DateTimeOfFirstRecord + (index * _TimeRecordInterval);
}