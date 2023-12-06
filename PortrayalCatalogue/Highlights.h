#pragma once
#include <string>
#include <vector>
#include "GraphicalHighlighting.h"

namespace S100
{
	class Highlights
	{
	public:
		std::vector<GraphicalHighlighting*> highlight;

		void GetContents(pugi::xml_node& node);
	};
}
