#include "stdafx.h"
#include "DatasetPermit.h"
#include "pugiSupportClass.h"

namespace S100{
	void DatasetPermit::GetContents(pugi::xml_node& node)
	{
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (ExXmlSupport().Compare(instructionName, "filename"))
            {
                filename = instruction.child_value();
            }
            else if (ExXmlSupport().Compare(instructionName, "editionNumber"))
            {
                std::string str = instruction.child_value();
                editionNumber = atoi(str.c_str());
            }
            else if (ExXmlSupport().Compare(instructionName, "issueDate"))
            {
                issueDate = S100_DateTime(instruction.child_value());
            }
            else if (ExXmlSupport().Compare(instructionName, "expiry"))
            {
                expiry = S100_Date(instruction.child_value());
            }
            else if (ExXmlSupport().Compare(instructionName, "encryptedKey"))
            {
                encryptedKey = instruction.child_value();
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
	}
}

