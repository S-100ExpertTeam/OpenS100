#pragma once
#include "../PortrayalCatalogue/CatalogItem.h"
#include <string>
#include <vector>
#include <memory>
#include "HighlightStyle.h"
#include "ViewingGroupReference.h"

namespace S100
{
	class GraphicalHighlighting :
		public Portrayal::CatalogItem
	{
	public:
		std::shared_ptr<HighlightStyle>  style;
		std::shared_ptr<bool> optional;
		std::shared_ptr<std::string> msg;
		std::vector<ViewingGroupReference> viewingGroup;

		void GetContents(pugi::xml_node& node);
	};
}
