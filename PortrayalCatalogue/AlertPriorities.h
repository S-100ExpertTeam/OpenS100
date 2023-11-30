#pragma once
#include "AlertPrioritySelection.h"
#include <string>
#include <vector>

#include "pugixml.hpp"

namespace S100
{
	class AlertPriorities
	{
	public:
		std::string label;
		std::vector<AlertPrioritySelection> priority;

		void GetContents(pugi::xml_node& node);
	};
}
