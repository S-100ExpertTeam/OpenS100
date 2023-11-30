#pragma once
#include "AlertPrioritySelection.h"

#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace S100
{
	class AlertPriorities
	{
	public:

		string label;
		vector<AlertPrioritySelection> priority;

        void AlertPriorities::GetContents(pugi::xml_node& node)
        {
            auto idAttri = node.attribute("label");
            if (idAttri != nullptr)
            {
                label = idAttri.value();
            }

            for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
            {
                const pugi::char_t* instructionName = instruction.name();

                if (!strcmp(instructionName, "priority"))
                {
                    AlertPrioritySelection ap; 
                    ap.GetContents(instruction);
                    priority.push_back(ap);
                }
            }
        }
	};

}

