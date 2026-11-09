#pragma once
#include "AlertPriority.h"
#include <string>
#include <memory>

#include "pugixml.hpp"

namespace S100
{
	class AlertPrioritySelection
	{
	public:
		AlertPriority alertPriority;
		std::shared_ptr<bool> isDefault;
		std::shared_ptr<bool> optional;

		void GetContents(pugi::xml_node& node);
	};
}
