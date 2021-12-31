#pragma once

#include <ctime>

namespace LatLonUtility
{

	class TimeUtil
	{
	public:
		TimeUtil();
		virtual ~TimeUtil();

	public:
		int year = 0;
		int mon = 0;
		int day = 0;
		int hour = 0;
		int min = 0;
		int sec = 0;
		int ms = 0;

	public:
		time_t Get_time_t();
		tm Get_tm();

		void Set(time_t value);
		void Set(tm value);
		void Set(int year, int mon, int day, int hour = 0, int min = 0, int sec = 0, int ms = 0);
	};

}