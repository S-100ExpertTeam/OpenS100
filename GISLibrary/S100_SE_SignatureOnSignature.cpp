#include "stdafx.h"
#include "S100_SE_SignatureOnSignature.h"

namespace S100 {
    void S100_SE_SignatureOnSignature::GetContents(pugi::xml_node& node)
    {
        Value = node.child_value();

        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "id"))
            {
                Id = instruction.child_value();
            }
            else if (!strcmp(instructionName, "certificateRef"))
            {
                CertificateRef = instruction.child_value();
            }
            else if (!strcmp(instructionName, "signatureref"))
            {
                Signatureref = instruction.child_value();
            }
        }
    }
}
