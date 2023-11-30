#pragma once
#include <string>
#include <vector>
#include "MessageItem.h"

using namespace std;

namespace S100
{
	class Messages
	{
    public:
		vector<MessageItem *> message;

        void Messages::GetContents(pugi::xml_node& node)
        {
            for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
            {
                const pugi::char_t* instructionName = instruction.name();

                if (!strcmp(instructionName, "message"))
                {
                    MessageItem* mi = new MessageItem();
                    mi->GetContents(instruction);
                    message.push_back(mi);
                }
            }
        }
	};

}
