#pragma once
#include <string>
#include <vector>
#include <memory>
#include "AlertPriority.h"
#include "AlertPriorities.h"

using namespace std;

namespace S100
{
	class AlertInfo
	{
	public:

        string msg;
        shared_ptr<string> highlight;

		shared_ptr<AlertPriority> priority;
		shared_ptr<AlertPriorities> priorities;

        void AlertInfo::GetContents(pugi::xml_node& node)
        {
            auto idAttri = node.attribute("msg");
            if (idAttri != nullptr)
            {
                msg = idAttri.value();
            }
            idAttri = node.attribute("highlight");
            if (idAttri != nullptr)
            {
                highlight = make_shared<string>(idAttri.value());
            }


            for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
            {
                const pugi::char_t* instructionName = instruction.name();

                if (!strcmp(instructionName, "priority"))
                {
                    priority = make_shared<AlertPriority>(AlertPriorityFromString(instruction.child_value()));
                }
                else if (!strcmp(instructionName, "priorities"))
                {
                    AlertPriorities ap;
                    ap.GetContents(instruction);
                    priorities = make_shared<AlertPriorities>(ap);
                }
            }
        }
	};

}

