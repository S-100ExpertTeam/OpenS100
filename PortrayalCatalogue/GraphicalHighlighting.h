#pragma once

#include "../PortrayalCatalogue/CatalogItem.h"

#include <string>
#include <vector>
#include <memory>
#include "HighlightStyle.h"
#include "ViewingGroupReference.h"

using namespace std;

namespace S100
{
	class GraphicalHighlighting:
		public Portrayal::CatalogItem
	{
    public:
		shared_ptr<HighlightStyle>  style;
		shared_ptr<bool> optional;
		shared_ptr<string> msg;
		vector<ViewingGroupReference> viewingGroup;

        void GraphicalHighlighting::GetContents(pugi::xml_node& node)
        {
            auto idAttri = node.attribute("id");
            if (idAttri != nullptr)
            {
                auto idValue = pugi::as_wide(idAttri.value());
                SetId(idValue);
            }
            idAttri = node.attribute("optional");
            if (idAttri != nullptr)
            {
                optional = make_shared<bool>((string)idAttri.value() == "true"? true : false);
            }
            idAttri = node.attribute("msg");
            if (idAttri != nullptr)
            {
                msg = make_shared<string>(idAttri.value());
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
                else if (!strcmp(instructionName, "viewingGroup"))
                {
                    ViewingGroupReference vg;
                    vg.GetContents(instruction);
                    viewingGroup.push_back(vg);
                }
            }
        }
	};

}
