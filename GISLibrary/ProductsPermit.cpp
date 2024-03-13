#include "stdafx.h"
#include "ProductsPermit.h"
#include "pugiSupportClass.h"

namespace S100 {
	void ProductsPermit::GetContents(pugi::xml_node& node)
	{
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (ExXmlSupport().Compare(instructionName, "product"))
            {
                ProductPermit permit;
                permit.GetContents(instruction);
                products.push_back(permit);
            }
        }
	}
}