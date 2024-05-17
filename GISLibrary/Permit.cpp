#include "stdafx.h"
#include "Permit.h"
#include "pugiSupportClass.h"

namespace S100 {

    void Permit::Open(std::string filePath)
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(filePath.c_str());
        pugi::xml_node nodeList = doc.first_child();

        const pugi::char_t* instructionName = nodeList.name();
        if (ExXmlSupport().Compare(instructionName, "permit"))
        {
            GetContents(nodeList);
        }
        else
        {

        }
    }

    void Permit::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (ExXmlSupport().Compare(instructionName, "header"))
            {
                permitHeader.GetContents(instruction);
            }
            else if (ExXmlSupport().Compare(instructionName, "products"))
            {
                productsPermit.GetContents(instruction);
            }
        }
    }
}