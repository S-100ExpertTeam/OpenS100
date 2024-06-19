#include "stdafx.h"
#include "AlertCommands.h"

namespace DrawingInstruction
{
	AlertCommands::~AlertCommands()
	{
		delete alertReference;
		alertReference = nullptr;
	}
	void AlertCommands::setAlertReference(const std::string& alertReference, const std::string& plan, const std::string& monitor)
	{
		delete this->alertReference;
		this->alertReference = new AlertReference(alertReference, plan, monitor);
	}

	void AlertCommands::parse(const std::string& key, std::string value)
	{
		if (key == "AlertReference")
		{
			alertReference->execute();
		}
	}

	void AlertCommands::executeCommands() const
	{
		if (alertReference) alertReference->execute();
	}

	AlertReference::AlertReference(std::string alertReference, std::string plan, std::string monitor)
		: alertReference(alertReference), plan(plan), monitor(monitor) {}
	
	void AlertReference::execute()  {
	}
	void AlertReference::parse(const std::string& input)
	{
	}
}
