#pragma once
#include <string>
#include <vector>
#include "GraphicalHighlighting.h"

using namespace std;

namespace S100
{
	class Highlights
	{
    public:
		vector<GraphicalHighlighting *> highlight;

        void Highlights::GetContents(pugi::xml_node& node)
        {
            for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
            {
                const pugi::char_t* instructionName = instruction.name();

                if (!strcmp(instructionName, "highlight"))
                {
                    GraphicalHighlighting* gh = new GraphicalHighlighting();
                    gh->GetContents(instruction);
                    highlight.push_back(gh);
                }
            }
        }
	};

}
