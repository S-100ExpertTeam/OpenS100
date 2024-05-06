#include "stdafx.h"
#include "SupportFileDiscoveryMetadata.h"
#include "S100_SE_SignatureOnData.h"

namespace S100 {
    void SupportFileDiscoveryMetadata::GetContents(pugi::xml_node& node)
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
                SupportFileSpecification sf;
                sf.GetContents(instruction);
                supportFileSpecification = std::make_shared<SupportFileSpecification>(sf);
            }
            else if (!strcmp(instructionName, "S100XC:dataType"))
            {
                DataType = SupportFileFormatFromString(instruction.child_value());
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
                CompressionFlag = ExXmlSupport().ParseStr2Bool(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:digitalSignatureReference"))
            {
                DigitalSignatureReference = S100_SE_DigitalSignatureReferenceFromString(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:digitalSignatureValue"))
            {
                auto childNode = instruction.first_child();
                auto childName = childNode.name();
                if (childName == "S100SE:S100_SE_SignatureOnData")
                {
                    S100_SE_SignatureOnData ds;
                    auto temp = ExXmlSupport().GetContentNode(instruction, "S100_SE_SignatureOnData", "S100SE");
                    ds.GetContents(temp);
                    DigitalSignatureValue.push_back(ds);

                }
                else if (childName == "S100SE:S100_SE_DigitalSignature")
                {
                    S100_SE_DigitalSignature ds;
                    auto temp = ExXmlSupport().GetContentNode(instruction, "S100_SE_SignatureOnData", "S100SE");
                    ds.GetContents(temp);
                    DigitalSignatureValue.push_back(ds);
                }
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
                resourcePurpose = std::make_shared<ResourcePurpose>(S100_ResourcePurposeFromString(instruction.child_value()));
            }
            else
            {
            }
        }
    }

    void SupportFileDiscoveryMetadata::SaveXmlNode(pugi::xml_node& node)
    {
        if (!FileName.empty())
        {
            auto child = node.append_child("S100XC:fileName");
            child.text().set(FileName.c_str());
        }
        {
            auto child = node.append_child("S100XC:revisionStatus");
            child.text().set(S100_SupportFileRevisionStatusToString(RevisionStatus).c_str());
        }
        {
            auto child = node.append_child("S100XC:editionNumber");
            child.text().set(std::to_string(EditionNumber).c_str());
        }
        if (IssueDate)
        {
            auto child = node.append_child("S100XC:issueDate");
            child.text().set(IssueDate->ToString().c_str());
        }
        if (supportFileSpecification)
        {
            auto child = node.append_child("S100XC:supportFileSpecification");
            supportFileSpecification->Save(child);
        }
        {
            auto child = node.append_child("S100XC:dataType");
            child.text().set(SupportFileFormatToString(DataType).c_str());
        }
        if (OtherDataTypeDescription)
        {
            auto child = node.append_child("S100XC:otherDataTypeDescription");
            child.text().set(OtherDataTypeDescription->c_str());

        }
        if (Comment)
        {
            auto child = node.append_child("S100XC:comment");
            child.text().set(Comment->c_str());
        }
        {
            auto child = node.append_child("S100XC:compressionFlag");
            child.text().set(ExXmlSupport().ParseBool2Str(CompressionFlag).c_str());
        }
        {
            auto child = node.append_child("S100XC:digitalSignatureReference");
            child.text().set(S100_SE_DigitalSignatureReferenceToString(DigitalSignatureReference).c_str());
        }
        if (!DigitalSignatureValue.empty())
        {
            for (int i = 0; i < DigitalSignatureValue.size(); i++)
            {
                auto child = node.append_child("S100XC:digitalSignatureValue");
                DigitalSignatureValue[i].Save(child);
            }
        }
        if (DefaultLocale)
        {
            auto child = node.append_child("S100XC:defaultLocale");
            DefaultLocale->Save(child);
        }
        if (!SupportedResource.empty())
        {
            for (int i = 0; i < SupportedResource.size(); i++)
            {
                auto child = node.append_child("S100XC:supportedResource");
                child.text().set(SupportedResource[i].c_str());
            }
        }
        if (resourcePurpose)
        {
            auto child = node.append_child("S100XC:resourcePurpose");
            child.text().set(S100_ResourcePurposeToString(*resourcePurpose).c_str());
        }
    }
}

    
