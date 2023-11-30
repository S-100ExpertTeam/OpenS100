#pragma once
#include <string>
#include <vector>
#include "AlertItem.h"

namespace S100
{
	class Alerts
	{
	public:
		std::vector<AlertItem*> alert;

		void GetContents(pugi::xml_node& node);
	};
}
