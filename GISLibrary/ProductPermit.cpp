#include "stdafx.h"
#include "ProductPermit.h"
#include "pugiSupportClass.h"

namespace S100 {
    void ProductPermit::GetContents(pugi::xml_node& node)
    {
        if (node.attribute("id")) //Add attribute value
            id = node.attribute("id").value();

        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (ExXmlSupport().Compare(instructionName, "datasetPermit"))
            {
                DatasetPermit permit;
                permit.GetContents(instruction);
                datasetPermit.push_back(permit);
            }
        }
    }
}