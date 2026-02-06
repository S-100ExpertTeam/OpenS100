#include "stdafx.h"
#include "AlertCommands.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace Part9a
{
	AlertReference::AlertReference(std::string alertReference, std::string plan, std::string monitor)
		: alertReference(alertReference), plan(plan), monitor(monitor) {}
	
	void AlertReference::init()
	{
		Command::init();
		alertReference.clear();
		plan.clear();
		monitor.clear();
	}

	void AlertReference::execute()  {
	}
	void AlertReference::parse(const std::string& input)
	{
		setPresent();
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
