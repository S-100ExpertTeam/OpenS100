#include "stdafx.h"
#include "TimeCommands.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace DrawingInstructions
{
	TimeCommands::~TimeCommands()
	{
		delete date;
		delete time;
		delete dateTime;
		delete timeValid;
		delete clearTimeCmd;
		date = nullptr;
		time = nullptr;
		dateTime = nullptr;
		timeValid = nullptr;
		clearTimeCmd = nullptr;
	}
	void TimeCommands::setDate(const std::string& begin, const std::string& end)
	{
		delete this->date;
		this->date = new Date(begin, end);
	}

	void TimeCommands::setTime(const std::string& begin, const std::string& end)
	{
		delete this->time;
		this->time = new Time(begin, end);
	}

	void TimeCommands::setDateTime(const std::string& begin, const std::string& end)
	{
		delete this->dateTime;
		this->dateTime = new DateTime(begin, end);
	}

	void TimeCommands::setTimeValid(const IntervalType closure)
	{
		delete this->timeValid;
		this->timeValid = new TimeValid(closure);
	}

	void TimeCommands::clearTime()
	{
		delete this->clearTimeCmd;
		this->clearTimeCmd = new ClearTime();
	}

	void TimeCommands::parse(const std::string& key, std::string value)
	{
		if (key == "Date")
		{
			//date->execute();
		}
		else if (key == "Time")
		{
			//time->execute();
		}
		else if (key == "DateTime")
		{
			//dateTime->execute();
		}
		else if (key == "TimeValid")
		{
			//timeValid->execute();
		}
		else if (key == "ClearTime")
		{
			//clearTimeCmd->execute();
		}
	}

	void TimeCommands::execute() const
	{
		if (date) date->execute();
		if (time) time->execute();
		if (dateTime) dateTime->execute();
		if (timeValid) timeValid->execute();
		if (clearTimeCmd) clearTimeCmd->execute();
	}

	Date::Date(const std::string& begin, const std::string& end) : begin(begin), end(end) {}

	void Date::init()
	{
		StateCommand::init();
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
		StateCommand::init();
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
		StateCommand::init();
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
		StateCommand::init();
		closure = IntervalType::none;
	}

	void TimeValid::execute() 
	{
	}

	void TimeValid::parse(const std::string& input)
	{
		setPresent();
		// TimeValid[:closure]
		closure = StateCommand::GetIntervalTypeFromString(input);
	}

	void ClearTime::init()
	{
		StateCommand::init();
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
