
#include "TimeUtil.h"

namespace LatLonUtility
{
	TimeUtil::TimeUtil()
	{

	}

	TimeUtil::~TimeUtil()
	{

	}

	time_t TimeUtil::Get_time_t()
	{
		auto local_tm = Get_tm();
		auto local_time = mktime(&local_tm);
		return local_time;
	}

	tm TimeUtil::Get_tm()
	{
		tm result;
		result.tm_year = year - 1900;
		result.tm_mon = mon - 1;
		result.tm_mday = day;
		result.tm_hour = hour;
		result.tm_min = min;
		result.tm_sec = sec;

		return result;
	}

	void TimeUtil::Set(time_t value)
	{
		tm t;
		localtime_s(&t,&value);
		Set(t);
	}

	void TimeUtil::Set(tm value)
	{
		year = value.tm_year + 1900;
		mon = value.tm_mon + 1;
		day = value.tm_mday;
		hour = value.tm_hour;
		min = value.tm_min;
		sec = value.tm_sec;
		ms = 0;
	}

	void TimeUtil::Set(int year, int mon, int day, int hour, int min, int sec, int ms)
	{
		this->year = year;
		this->mon = mon;
		this->day = day;
		this->hour = hour;
		this->min = min;
		this->sec = sec;
		this->ms = ms;
	}
}