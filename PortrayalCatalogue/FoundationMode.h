#pragma once
#include <pugixml.hpp>

#include <unordered_map>
#include <string>

namespace Portrayal
{
	class ViewingGroup;
	class ViewingGroups;

	class FoundationMode
	{
	public:
		FoundationMode();
		virtual ~FoundationMode();

	private:
		std::unordered_map<std::string, ViewingGroup*> viewingGroup;
		std::vector<ViewingGroup*> viewingGroup_v;

	public:
		void GetContents(pugi::xml_node& node, ViewingGroups* view);

		void SetViewingGroup(const std::string& key, ViewingGroup* value);
		void SetViewingGroup(const std::wstring& key, ViewingGroup* value);
		ViewingGroup* GetViewingGroup(const std::string& key);
		ViewingGroup* GetViewingGroup(const std::wstring& key);

		std::vector<ViewingGroup*>* GetViewingGroupVector();

		bool HasVewingGroup(const std::string& key);
		bool HasVewingGroup(const std::wstring& key);
	};
}
