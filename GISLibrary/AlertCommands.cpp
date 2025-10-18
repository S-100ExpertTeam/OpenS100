#include "stdafx.h"
#include "AlertCommands.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace DrawingInstructions
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

	void AlertCommands::execute() const
	{
		if (alertReference) alertReference->execute();
	}

	AlertReference::AlertReference(std::string alertReference, std::string plan, std::string monitor)
		: alertReference(alertReference), plan(plan), monitor(monitor) {}
	
	void AlertReference::init()
	{
		StateCommand::init();
		alertReference.clear();
		plan.clear();
		monitor.clear();
	}

	void AlertReference::execute()  {
	}
	void AlertReference::parse(const std::string& input)
	{
		// AlertReference[:alertReference[,plan[,monitor]]] 
		auto tokens = LatLonUtility::Split(input, ":");

		if (tokens.size() > 1) 
		{
			alertReference = tokens[1];
		}
		
		if (tokens.size() > 2) 
		{
			plan = tokens[2];
		}
		
		if (tokens.size() > 3) 
		{
			monitor = tokens[3];
		}
	}
}
