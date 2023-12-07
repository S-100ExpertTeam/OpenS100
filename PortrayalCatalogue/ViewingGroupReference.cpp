#include "stdafx.h"
#include "ViewingGroupReference.h"
#include "pugixml.hpp"

using namespace S100;

void ViewingGroupReference::GetContents(pugi::xml_node& node)
{
    auto idAttri = node.attribute("ref");
    if (idAttri != nullptr)
    {
        ref = idAttri.value();
    }
}
