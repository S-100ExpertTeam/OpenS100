#include "stdafx.h"
#include "ViewingGroupLayer.h"
#include "ViewingGroup.h"

namespace Portrayal
{
	ViewingGroupLayer::ViewingGroupLayer()
	{

	}

	ViewingGroupLayer::~ViewingGroupLayer()
	{
		for (auto i = viewingGroup_v.begin(); i != viewingGroup_v.end(); i++)
		{
			delete *i;
		}
	}

	void ViewingGroupLayer::GetContents(pugi::xml_node& node)
	{
		auto ischild = node.child_value();
		if (strcmp(ischild, ""))
		{
			value = node.child_value();
			return;
		}

		auto idNode = node.first_attribute();
		if (idNode != nullptr)
		{
			auto idName = idNode.name();
			if (!strcmp(idName, "id"))
			{
				SetId(std::string(idNode.value()));
			}
		}

		for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();

			if (!strcmp(instructionName, "description"))
			{
				S100_Description* desc = new S100_Description();
				desc->GetContents(instruction);
				AddDescription(desc);
			}
			else if (!strcmp(instructionName, "viewingGroup"))
			{
				ViewingGroup* viewingGroupNode = new ViewingGroup();
				viewingGroupNode->GetContents(instruction);
				viewingGroup_v.push_back(viewingGroupNode);
				viewingGroup.insert({ viewingGroupNode->GetId(), viewingGroupNode });
			}
		}
	}

	void ViewingGroupLayer::AddViewingGroup(const std::string& key, ViewingGroup* val)
	{
		viewingGroup[key] = val;
	}

	void ViewingGroupLayer::AddViewingGroup(const std::wstring& key, ViewingGroup* val)
	{
		viewingGroup[toUtf8(key)] = val;
	}

	void ViewingGroupLayer::SetViewingGroup(std::unordered_map<std::string, ViewingGroup*> val)
	{
		viewingGroup = val;
	}

	ViewingGroup* ViewingGroupLayer::GetViewingGroup(const std::string& key)
	{
		if (HasViewingGroup(key))
		{
			return viewingGroup[key];
		}
		return nullptr;
	}

	ViewingGroup* ViewingGroupLayer::GetViewingGroup(const std::wstring& key)
	{
		return GetViewingGroup(toUtf8(key));
	}

	bool ViewingGroupLayer::HasViewingGroup(const std::string& key)
	{
		return viewingGroup.find(key) != viewingGroup.end();
	}

	bool ViewingGroupLayer::HasViewingGroup(const std::wstring& key)
	{
		return HasViewingGroup(toUtf8(key));
	}

	std::vector<ViewingGroup*>* ViewingGroupLayer::GetViewingGroupVector()
	{
		return &viewingGroup_v;
	}

	std::string  ViewingGroupLayer::GetValue()  { return value; }
	std::wstring ViewingGroupLayer::GetValueW() { return toWide(value); }
}
