#pragma once
#include "ViewingGroup.h"

#include <unordered_map>
#include <vector>
#include <string>

namespace Portrayal
{
	class ViewingGroups
	{
	public:
		ViewingGroups();
		virtual ~ViewingGroups();

	private:
		std::vector<ViewingGroup*> viewingGroup_v;
		std::unordered_map<std::string, ViewingGroup*> viewingGroup;

	public:
		void GetContents(pugi::xml_node& node);
		void AddViewingGroup(ViewingGroup* value);
		void SetViewingGroup(std::vector<ViewingGroup*> value);
		ViewingGroup* GetViewingGroup(int index);
		ViewingGroup* GetViewingGroup_id(const std::string& id);
		ViewingGroup* GetViewingGroup_id(const std::wstring& id);

		std::vector<ViewingGroup*>* GetViewingGroup();
	};
}
