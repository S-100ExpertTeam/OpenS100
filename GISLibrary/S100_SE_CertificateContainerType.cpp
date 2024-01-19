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


    void S100_SE_CertificateContainerType::Save(pugi::xml_node& node)
    {
        if (SchemeAdministrator || !Certificate.empty())
        {
            if (SchemeAdministrator)
            {
                auto child = node.append_child("S100SE:schemeAdministrator");
                child.text().set(SchemeAdministrator->c_str());
            }
            if (!Certificate.empty())
            {
                for (int i = 0; i < Certificate.size(); i++)
                {
                    auto child = node.append_child("S100SE:certificate");
                    child.text().set(Certificate[i].c_str());
                }
            }
        }
    }
}
