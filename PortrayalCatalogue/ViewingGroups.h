#pragma once
#include "ViewingGroup.h"

#include <vector>

namespace Portrayal
{
	class ViewingGroups
	{
	public:
		ViewingGroups();
		virtual ~ViewingGroups();

	private:
		std::vector<ViewingGroup*> viewingGroup;

	public:
		void GetContents(pugi::xml_node& node);
		void AddViewingGroup(ViewingGroup* value);
		void SetViewingGroup(std::vector<ViewingGroup*> value);
		ViewingGroup* GetViewingGroup(int index);
		std::vector<ViewingGroup*> GetViewingGroup();
	};
}