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
		std::unordered_map<std::string, ViewingGroup*> viewingGroup;

		std::string value;

	public:
		void GetContents(pugi::xml_node& node);
		void AddViewingGroup(const std::string& key, ViewingGroup* value);
		void AddViewingGroup(const std::wstring& key, ViewingGroup* value);
		void SetViewingGroup(std::unordered_map<std::string, ViewingGroup*> value);

		ViewingGroup* GetViewingGroup(const std::string& key);
		ViewingGroup* GetViewingGroup(const std::wstring& key);
		std::vector<ViewingGroup*>* GetViewingGroupVector();

		std::string  GetValue();
		std::wstring GetValueW();

		bool HasViewingGroup(const std::string& key);
		bool HasViewingGroup(const std::wstring& key);
	};
}
