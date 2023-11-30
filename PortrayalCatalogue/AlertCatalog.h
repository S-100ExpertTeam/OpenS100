#pragma once
#include <string>
#include "Messages.h"
#include "Highlights.h"
#include "Alerts.h"

namespace S100
{
	class AlertCatalog
	{
	public:
		std::string Version;
		Messages messages;
		Highlights highlights;
		Alerts alerts;

		void Open(std::string filePath);
		void GetContents(pugi::xml_node& node);
	};
}
