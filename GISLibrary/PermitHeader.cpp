#include "stdafx.h"
#include "PermitHeader.h"

namespace S100
{
	void PermitHeader::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (ExXmlSupport().Compare(instructionName, "issueDate"))
            {
                issueDate = S100_DateTime(instruction.child_value());
            }
            else if (ExXmlSupport().Compare(instructionName, "dataserverName"))
            {
                dataServerName = instruction.child_value();
            }
            else if (ExXmlSupport().Compare(instructionName, "dataserverIdentifier"))
            {
                dataServerIdentifier = instruction.child_value();
            }
            else if (ExXmlSupport().Compare(instructionName, "version"))
            {
                version = instruction.child_value();
            }
            else if (ExXmlSupport().Compare(instructionName, "userpermit"))
            {
                userpermit = instruction.child_value();
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }
}