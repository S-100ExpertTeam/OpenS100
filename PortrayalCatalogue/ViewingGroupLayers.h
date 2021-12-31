 #pragma once

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <vector>

namespace Portrayal
{

	class ViewingGroupLayer;
	class ViewingGroupLayers
	{
	public:
		ViewingGroupLayers();
		virtual ~ViewingGroupLayers();

	private:
		std::vector<ViewingGroupLayer*> layer;

	public:
		void GetContents(pugi::xml_node& node);
		void AddViewingGroupLayer(ViewingGroupLayer* value);
		ViewingGroupLayer* GetViewingGroupLayer(int index);
		int GetCountOfViewingGroupLayer(); // layer size
	};
}