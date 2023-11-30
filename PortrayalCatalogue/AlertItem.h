#pragma once
#include "../PortrayalCatalogue/CatalogItem.h"

#include <string>
#include <vector>
#include <memory>
#include "AlertInfo.h"

using namespace std;

namespace S100
{
	class AlertItem :
		public Portrayal::CatalogItem
	{
	public:

		shared_ptr<AlertInfo> routeMonitor;
		shared_ptr<AlertInfo> routePlan;

        void AlertItem::GetContents(pugi::xml_node& node)
        {
            auto idAttri = node.attribute("id");
            if (idAttri != nullptr)
            {
                auto idValue = pugi::as_wide(idAttri.value());
                SetId(idValue);
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
                else if (!strcmp(instructionName, "routeMonitor"))
                {
                    AlertInfo ai;
                    ai.GetContents(instruction);
                    routeMonitor = make_shared<AlertInfo>(ai);
                }
                else if (!strcmp(instructionName, "routePlan"))
                {
                    AlertInfo vg;
                    vg.GetContents(instruction);
                    routePlan = make_shared<AlertInfo>(vg);
                }
            }
        }
	};

}
