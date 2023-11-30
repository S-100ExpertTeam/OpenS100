#pragma once
#include <string>
#include <vector>
#include "AlertItem.h"

using namespace std;

namespace S100
{
	class Alerts
	{
	public:
		vector<AlertItem*> alert;

        void Alerts::GetContents(pugi::xml_node& node)
        {
            for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
            {
                const pugi::char_t* instructionName = instruction.name();

                if (!strcmp(instructionName, "alert"))
                {
                    AlertItem* al = new AlertItem();
                    al->GetContents(instruction);
                    alert.push_back(al);
                }
            }
        }
	};

}
