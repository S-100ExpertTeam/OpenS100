#include "stdafx.h"
#include "AlertPrioritySelection.h"
#include "pugixml.hpp"

using namespace S100;

void AlertPrioritySelection::GetContents(pugi::xml_node& node)
{
    auto idAttri = node.attribute("default");
    if (idAttri != nullptr)
    {
        default = std::make_shared<bool>((std::string)idAttri.value() == "true" ? true : false);
    }
    idAttri = node.attribute("optional");
    if (idAttri != nullptr)
    {
        optional = std::make_shared<bool>((std::string)idAttri.value() == "true" ? true : false);
    }

    alertPriority = AlertPriorityFromString(node.child_value());
}
