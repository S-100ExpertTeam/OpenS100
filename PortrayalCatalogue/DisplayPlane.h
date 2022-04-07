#pragma once
#include "CatalogItem.h"

namespace Portrayal
{
	class DisplayPlane :
		public CatalogItem
	{
	public:
		DisplayPlane();
		virtual ~DisplayPlane();

	private:
		int order = 0;
	public:
		int GetOrder();
		void SetOrder(int value);
		void GetContents(pugi::xml_node& node);
	};
}