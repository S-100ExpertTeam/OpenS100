#pragma once
#include "AlertPriority.h"

#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace S100
{
	class AlertPrioritySelection 
	{
	public:

		AlertPriority alertPriority;
		shared_ptr<bool> default;
		shared_ptr<bool> optional;

        void AlertPrioritySelection::GetContents(pugi::xml_node& node)
        {
            auto idAttri = node.attribute("default");
            if (idAttri != nullptr)
            {
                default = make_shared<bool>((string)idAttri.value() == "true" ? true : false);
            }
            idAttri = node.attribute("optional");
            if (idAttri != nullptr)
            {
                optional = make_shared<bool>((string)idAttri.value() == "true" ? true : false);
            }

            alertPriority = AlertPriorityFromString(node.child_value());
        }
	};
}

