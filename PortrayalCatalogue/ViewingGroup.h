#pragma once
#include "CatalogItem.h"

namespace Portrayal
{
	class ViewingGroup :
		public CatalogItem
	{
	public:
		ViewingGroup();
		virtual ~ViewingGroup();

	public:
		void GetContents(pugi::xml_node& node);
	};
}