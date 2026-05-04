#include "stdafx.h"
#include "Text.h"


using namespace S100;

void Text::GetContents(pugi::xml_node& node)
{
    auto idAttri = node.attribute("language");
    if (idAttri != nullptr)
    {
        Language = std::make_shared<std::string>(idAttri.value());
    }

    Value = node.child_value();
}
