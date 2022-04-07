#include "stdafx.h"
#include "ViewingGroupLayers.h"
#include "ViewingGroupLayer.h"

namespace Portrayal
{
	ViewingGroupLayers::ViewingGroupLayers() 
	{

	}

	ViewingGroupLayers::~ViewingGroupLayers() 
	{
		for (auto i = layer.begin(); i != layer.end(); i++)
		{
			delete *i;
		}
	}

	void ViewingGroupLayers::GetContents(pugi::xml_node& node )
	{
		for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName,"viewingGroupLayer"))
			{
				ViewingGroupLayer* layervalue = new ViewingGroupLayer();
				layervalue->GetContents(instruction);
				layer.push_back(layervalue);
			}
		}
	}

	void ViewingGroupLayers::AddViewingGroupLayer(ViewingGroupLayer* value) 
	{
		layer.push_back(value);
	}

	ViewingGroupLayer* ViewingGroupLayers::GetViewingGroupLayer(int index)
	{
		return layer.at(index);
	}

	int ViewingGroupLayers::GetCountOfViewingGroupLayer()
	{
		return (int)layer.size();
	} 

	std::vector<ViewingGroupLayer*>* ViewingGroupLayers::GetViewingGroupLayerVector()
	{
		return &layer;
	}
}