#include "stdafx.h"
#include "S100_SE_CertificateContainerType.h"

namespace S100 {

    void S100_SE_CertificateContainerType::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "S100SE:schemeAdministrator"))
            {
                SchemeAdministrator = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100SE:certificate"))
            {
                Certificate.push_back(Base64String(instruction.child_value()));
            }
            else
            {
            }
        }
    }
}
