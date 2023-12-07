#pragma once
#include "../PortrayalCatalogue/CatalogItem.h"
#include <string>
#include <vector>
#include <memory>
#include "AlertInfo.h"

namespace S100
{
	class AlertItem :
		public Portrayal::CatalogItem
	{
	public:
		std::shared_ptr<AlertInfo> routeMonitor;
		std::shared_ptr<AlertInfo> routePlan;

		void GetContents(pugi::xml_node& node);
	};
}
