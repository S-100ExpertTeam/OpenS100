#pragma once
#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"


#include <unordered_map>
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
		std::unordered_map<std::wstring, ViewingGroup*> viewingGroup;
		std::vector<ViewingGroup*> viewingGroup_v;

	public:
		void GetContents(pugi::xml_node& node, ViewingGroups* view);

		void SetViewingGroup(std::wstring key, ViewingGroup* value);
		ViewingGroup* GetViewingGroup(std::wstring key);

		std::vector<ViewingGroup*>* GetViewingGroupVector();

		bool HasVewingGroup(std::wstring key);
	};
}