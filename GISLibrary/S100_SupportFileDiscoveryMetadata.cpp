#include "stdafx.h"
#include "S100_SupportFileDiscoveryMetadata.h"

namespace S100 {
    void S100_SupportFileDiscoveryMetadata::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "S100XC:fileName"))
            {
                FileName = instruction.child_value();
            }
            else if (!strcmp(instructionName, "S100XC:revisionStatus"))
            {
                RevisionStatus = S100_SupportFileRevisionStatusFromString(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:editionNumber"))
            {
                EditionNumber = atoi(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:issueDate"))
            {
                IssueDate = std::make_shared<S100_Date>(S100_Date(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:supportFileSpecification"))
            {
                S100_SupportFileSpecification sf;
                sf.GetContents(instruction);
                SupportFileSpecification = std::make_shared<S100_SupportFileSpecification>(sf);
            }
            else if (!strcmp(instructionName, "S100XC:dataType"))
            {
                DataType = S100_SupportFileFormatFromString(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:otherDataTypeDescription"))
            {
                OtherDataTypeDescription = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:comment"))
            {
                Comment = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:compressionFlag"))
            {
                CompressionFlag = ParseStr2Bool(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:digitalSignatureReference"))
            {
                DigitalSignatureReference = S100_SE_DigitalSignatureReferenceFromString(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:digitalSignatureValue"))
            {
                S100_SE_DigitalSignature ds;
                auto temp = GetContentNode(instruction, "S100_SE_SignatureOnData", "S100SE");
                ds.GetContents(temp);
                DigitalSignatureValue.push_back(ds);
            }
            else if (!strcmp(instructionName, "S100XC:defaultLocale"))
            {
                PT_Locale loc;
                loc.GetContents(instruction);
                DefaultLocale = std::make_shared<PT_Locale>(loc);
            }
            else if (!strcmp(instructionName, "S100XC:supportedResource"))
            {
                SupportedResource.push_back(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:resourcePurpose"))
            {
                ResourcePurpose = std::make_shared<S100_ResourcePurpose>(S100_ResourcePurposeFromString(instruction.child_value()));
            }
            else
            {
            }
        }
    }
}
