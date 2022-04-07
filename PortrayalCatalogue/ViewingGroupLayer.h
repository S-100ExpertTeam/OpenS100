#pragma once
#include "CatalogItem.h"
#include "ViewingGroup.h"

#include <unordered_map>
#include <string>

namespace Portrayal
{
	class ViewingGroupLayer :
		public CatalogItem
	{
	public:
		ViewingGroupLayer();
		virtual ~ViewingGroupLayer();

	private:
		std::vector<ViewingGroup*> viewingGroup_v;
		std::unordered_map<std::wstring, ViewingGroup*> viewingGroup;
		
		std::wstring value;
	public:
		void GetContents(pugi::xml_node& node);
		void AddViewingGroup(std::wstring key,ViewingGroup* value);
		void SetViewingGroup(std::unordered_map<std::wstring, ViewingGroup*> value);

		ViewingGroup* GetViewingGroup(std::wstring value);
		std::vector<ViewingGroup*>* GetViewingGroupVector();

		std::wstring GetValue();
		
		bool HasViewingGroup(std::wstring value);
	};
}