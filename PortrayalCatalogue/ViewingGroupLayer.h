#pragma once
#include "CatalogItem.h"

#include <unordered_map>
#include <string>

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
		std::vector<ViewingGroup*> viewingGroup_v;
		std::unordered_map<std::wstring, ViewingGroup*> viewingGroup;
		
		std::string value;
	public:
		void GetContents(pugi::xml_node& node);
		void AddViewingGroup(std::wstring key,ViewingGroup* value);
		void SetViewingGroup(std::unordered_map<std::wstring, ViewingGroup*> value);

		ViewingGroup* GetViewingGroup(std::wstring value);
		bool HasViewingGroup(std::wstring value);
	};
}