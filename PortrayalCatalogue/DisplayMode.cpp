#include "stdafx.h"
#include "DisplayMode.h"
#include "ViewingGroupLayer.h"
namespace Portrayal
{
	DisplayMode::DisplayMode()
	{

	}

	DisplayMode::~DisplayMode()
	{
		for (auto i = viewingGroupLayerVector.begin(); i != viewingGroupLayerVector.end(); i++)
		{
			delete *i;
		}
	}
	
	void DisplayMode::SetViewingGroupLayer(int key , ViewingGroupLayer* value) 
	{
		viewingGroupLayer.insert({key,value });
	}

	void DisplayMode::SetViewingGroupLayer(std::unordered_map<int, ViewingGroupLayer*> value)
	{
		viewingGroupLayer = value;
	}

	ViewingGroupLayer* DisplayMode::GetViewingGroupLayer(int value)
	{
		return viewingGroupLayer[value];
	}

	std::unordered_map<int, ViewingGroupLayer*> DisplayMode::GetViewingGroupLayer()
	{
		return viewingGroupLayer;
	}

	bool DisplayMode::HasViewingGroupLayer(int value)
	{
		auto islayer = viewingGroupLayer.find(value);
		if (islayer!= viewingGroupLayer.end())
		{
			return true;
		}
		return false;
	}

	void DisplayMode::GetContents(pugi::xml_node& node) 
	{
		auto idNode = node.first_attribute();
		if (idNode != nullptr)
		{
			auto idName = idNode.name();
			if (!strcmp(idName, "id"))
			{
				SetId(pugi::as_wide(idNode.value()));
			}
		}

		for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName,"description"))
			{
				S100_Description* desc = new S100_Description();
				desc->GetContents(instruction);
				AddDescription(desc);
			}
		
			if (!strcmp(instructionName, "viewingGroupLayer")) 
			{
				ViewingGroupLayer* viewing = new ViewingGroupLayer();
				viewing->GetContents(instruction);
				viewingGroupLayerVector.push_back(viewing);
			}
		}
	}

	std::vector<ViewingGroupLayer*>* DisplayMode::GetViewingGroupLayerVector()
	{
		return &viewingGroupLayerVector;
	}
}
