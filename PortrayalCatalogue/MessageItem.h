#pragma once
#include "Text.h"
#include "../PortrayalCatalogue/CatalogItem.h"
#include <string>
#include <vector>

namespace S100
{
	class MessageItem :
		public Portrayal::CatalogItem
	{
	public:
		std::shared_ptr<std::string> Icon;
		std::vector<Text> text;

		void GetContents(pugi::xml_node& node);
	};
}
