#include "stdafx.h"
#include "TimeCommands.h"
namespace DrawingInstruction
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

	void TimeCommands::setTimeValid(const std::string& closure)
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

	void TimeCommands::executeCommands() const
	{
		if (date) date->execute();
		if (time) time->execute();
		if (dateTime) dateTime->execute();
		if (timeValid) timeValid->execute();
		if (clearTimeCmd) clearTimeCmd->execute();
	}

	Date::Date(const std::string& begin, const std::string& end) : begin(begin), end(end) {}

	void  Date::execute() {
	}

	void Date::parse(const std::string& input)
	{
	}

	Time::Time(const std::string& begin, const std::string& end) : begin(begin), end(end) {}

	void Time::execute() 
	{
	}

	void Time::parse(const std::string& input)
	{
	}

	DateTime::DateTime(const std::string& begin, const std::string& end) : begin(begin), end(end) {}

	void DateTime::execute() 
	{
	}

	void DateTime::parse(const std::string& input)
	{
	}

	TimeValid::TimeValid(const std::string& closure) : closure(closure) {}

	void TimeValid::execute() 
	{
	}

	void TimeValid::parse(const std::string& input)
	{
	}

	void ClearTime::execute() 
	{
	}

	ClearTime::ClearTime()
	{
	}

	void ClearTime::parse(const std::string& input)
	{
	}




}
