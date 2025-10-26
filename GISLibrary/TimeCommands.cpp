#include "stdafx.h"
#include "TimeCommands.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace Part9a
{

	Date::Date(const std::string& begin, const std::string& end) : begin(begin), end(end) {}

	void Date::init()
	{
		Command::init();
		begin = "-…";
		end = "…";
	}	

	void  Date::execute() {
	}

	void Date::parse(const std::string& input)
	{
		setPresent();
		// Date:[begin][,end] 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() == 2) {
			begin = tokens[0];
			end = tokens[1];
		} else if (tokens.size() == 1) {
			begin = tokens[0];
			end = "…"; // Default end if not specified
		} else {
			init(); // Reset to default if parsing fails
		}
	}

	Time::Time(const std::string& begin, const std::string& end) : begin(begin), end(end) {}

	void Time::init()
	{
		Command::init();
		begin = "-…";
		end = "…";
	}	

	void Time::execute() 
	{
	}

	void Time::parse(const std::string& input)
	{
		setPresent();
		// Time:[begin][,end] 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() == 2) {
			begin = tokens[0];
			end = tokens[1];
		}
		else if (tokens.size() == 1) {
			begin = tokens[0];
			end = "…"; // Default end if not specified
		}
		else {
			init(); // Reset to default if parsing fails
		}
	}

	DateTime::DateTime(const std::string& begin, const std::string& end) : begin(begin), end(end) {}

	void DateTime::init()
	{
		Command::init();
		begin = "-…";
		end = "…";
	}

	void DateTime::execute() 
	{
	}

	void DateTime::parse(const std::string& input)
	{
		setPresent();
		// DateTime:[begin][,end] 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() == 2) {
			begin = tokens[0];
			end = tokens[1];
		}
		else if (tokens.size() == 1) {
			begin = tokens[0];
			end = "…"; // Default end if not specified
		}
		else {
			init(); // Reset to default if parsing fails
		}
	}

	TimeValid::TimeValid(const IntervalType closure) : closure(closure) {}

	void TimeValid::init()
	{
		Command::init();
		closure = IntervalType::none;
	}

	void TimeValid::execute() 
	{
	}

	void TimeValid::parse(const std::string& input)
	{
		setPresent();
		// TimeValid[:closure]
		closure = Command::GetIntervalTypeFromString(input);
	}

	void ClearTime::init()
	{
		Command::init();
	}

	void ClearTime::execute() 
	{
	}

	void ClearTime::parse(const std::string& input)
	{
		setPresent();
		// ClearTime 
	}




}
