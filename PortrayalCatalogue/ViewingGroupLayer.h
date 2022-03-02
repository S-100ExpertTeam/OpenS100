#pragma once
#include "CatalogItem.h"

#include <unordered_map>

namespace Portrayal
{
	class ViewingGroup;

	class ViewingGroupLayer :
		public CatalogItem
	{
	public:
		ViewingGroupLayer();
		virtual ~ViewingGroupLayer();

	private:
		std::vector<ViewingGroup*> viewingGroupVector;
		std::unordered_map<int, ViewingGroup*> viewingGroup;
		
	public:
		void GetContents(pugi::xml_node& node);
		void AddViewingGroup(int key,ViewingGroup* value);
		void SetViewingGroup(std::unordered_map<int, ViewingGroup*> value);

		ViewingGroup* GetViewingGroup(int value);
		bool HasViewingGroup(int value);
	};
}