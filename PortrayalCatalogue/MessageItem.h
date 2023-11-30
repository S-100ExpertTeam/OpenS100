#pragma once
#include "Text.h"
#include "../PortrayalCatalogue/CatalogItem.h"

#include <string>
#include <vector>
#include <memory>



using namespace std;

namespace S100
{
	class MessageItem:
		public Portrayal::CatalogItem
	{
    public:
		shared_ptr<string> Icon;
		vector<Text> text;

        void MessageItem::GetContents(pugi::xml_node& node)
        {
            auto idAttri = node.attribute("id");
            if (idAttri != nullptr)
            {
                auto idValue = pugi::as_wide(idAttri.value());
                SetId(idValue);
            }
            idAttri = node.attribute("icon");
            if (idAttri != nullptr)
            {
                Icon = make_shared<string>(idAttri.value());
            }
            

            for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
            {
                const pugi::char_t* instructionName = instruction.name();

                if (!strcmp(instructionName, "description"))
                {
                    S100_Description* des = new S100_Description();
                    des->GetContents(instruction);
                    AddDescription(des);
                }
                else if (!strcmp(instructionName, "text"))
                {
                    Text txt;
                    txt.GetContents(instruction);
                    text.push_back(txt);
                }
            }
        }
	};
}