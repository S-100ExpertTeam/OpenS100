#pragma once
#include <string>
#include <vector>
#include <memory>
#include "AlertPriority.h"
#include "AlertPriorities.h"

#include "pugixml.hpp"

namespace S100
{
	class AlertInfo
	{
	public:
		std::string msg;
		std::shared_ptr<std::string> highlight;
		std::shared_ptr<AlertPriority> priority;
		std::shared_ptr<AlertPriorities> priorities;

		void GetContents(pugi::xml_node& node);
	};
}
