#pragma once
#include "CatalogItem.h"

#include <unordered_map>

namespace Portrayal
{
	class ViewingGroupLayer;
	class DisplayMode :
		public CatalogItem
	{
	public:
		DisplayMode();
		virtual ~DisplayMode();

	private:
		std::vector<ViewingGroupLayer*> viewingGroupLayerVector;
		std::unordered_map<int, ViewingGroupLayer*> viewingGroupLayer;
	
	public:
		void SetViewingGroupLayer(int key, ViewingGroupLayer* value);
		void SetViewingGroupLayer(std::unordered_map<int, ViewingGroupLayer*> value);
		ViewingGroupLayer* GetViewingGroupLayer(int value);
		std::unordered_map<int, ViewingGroupLayer*> GetViewingGroupLayer();
		bool HasViewingGroupLayer(int value);

		std::vector<ViewingGroupLayer*>* GetViewingGroupLayerVector();

		void GetContents(pugi::xml_node& node);

	};
}