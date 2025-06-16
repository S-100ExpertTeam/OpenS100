#pragma once

#include "PrimitiveTypes.h"

#include <variant>

using S100_TM_Instant = std::variant<int, double, std::string>;

//class TM_Instant
//{
//public:
//	TM_Instant(Date date);
//	TM_Instant(Time time);
//	TM_Instant(DateTime dateTime);
//	virtual ~TM_Instant();

//protected:
//	std::variant<Date, Time, DateTime> value;

//public:
//	Date GetDate() const;
//	void SetDate(const Date date);

//	Time GetTime() const;
//	void SetTime(const Time time);

//	DateTime GetDateTime() const;
//	void SetDateTime(const DateTime dateTime);
//};