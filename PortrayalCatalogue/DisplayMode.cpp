#include "stdafx.h"
#include "DisplayMode.h"

namespace Portrayal
{
	DisplayMode::DisplayMode()
	{

	}

	DisplayMode::~DisplayMode()
	{

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
}
