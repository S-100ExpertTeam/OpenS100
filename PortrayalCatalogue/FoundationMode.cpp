#include "stdafx.h"
#include "FoundationMode.h"
#include "ViewingGroup.h"
#include "ViewingGroups.h"

namespace Portrayal
{
	FoundationMode::FoundationMode()
	{

	}

	FoundationMode::~FoundationMode()
	{

	}

	void FoundationMode::GetContents(pugi::xml_node& node, ViewingGroups* view)
	{
		for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName, "viewingGroup"))
			{
				std::string key = instruction.child_value();
				auto Viewing = view->GetViewingGroup_id(key);
				viewingGroup.insert({ key, Viewing });
				viewingGroup_v.push_back(Viewing);
			}
		}
	}

	void FoundationMode::SetViewingGroup(const std::string& key, ViewingGroup* value)
	{
		viewingGroup.insert({ key, value });
	}

	void FoundationMode::SetViewingGroup(const std::wstring& key, ViewingGroup* value)
	{
		viewingGroup.insert({ toUtf8(key), value });
	}

	ViewingGroup* FoundationMode::GetViewingGroup(const std::string& key)
	{
		return viewingGroup[key];
	}

	ViewingGroup* FoundationMode::GetViewingGroup(const std::wstring& key)
	{
		return GetViewingGroup(toUtf8(key));
	}

	bool FoundationMode::HasVewingGroup(const std::string& key)
	{
		return viewingGroup.find(key) != viewingGroup.end();
	}

	bool FoundationMode::HasVewingGroup(const std::wstring& key)
	{
		return HasVewingGroup(toUtf8(key));
	}

	std::vector<ViewingGroup*>* FoundationMode::GetViewingGroupVector()
	{
		return &viewingGroup_v;
	}
}
