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
		int value;

	public:
		void GetContents(pugi::xml_node& node);
	};
}