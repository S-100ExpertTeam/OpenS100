#include "stdafx.h"
#include "CatalogueDiscoveryMetadata.h"
#include "pugiSupportClass.h"

namespace S100 {
    void CatalogueDiscoveryMetadata::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "S100XC:fileName"))
            {
                FileName = instruction.child_value();
            }
            else if (!strcmp(instructionName, "S100XC:Purpose"))
            {
                if(!std::string(instruction.child_value()).empty())
                    purpose = std::make_shared<Purpose>(S100_PurposeFromString(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:EditionNumber"))
            {
                EditionNumber = atoi(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:Scope"))
            {
                if (!std::string(instruction.child_value()).empty())
                    Scope = S100_CatalogueScopeFromString(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:VersionNumber"))
            {
                VersionNumber = instruction.child_value();
            }
            else if (!strcmp(instructionName, "S100XC:IssueDate"))
            {
                IssueDate = S100_Date(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:ProductSpecification"))
            {
                ProductSpecification ps;
                ps.GetContents(instruction);
                productSpecification = ps;
            }
            else if (!strcmp(instructionName, "S100XC:DigitalSignatureReference"))
            {
                if(!std::string(instruction.child_value()).empty())
                    DigitalSignatureReference = S100_SE_DigitalSignatureReferenceFromString(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:DigitalSignatureValue"))
            {
                S100_SE_DigitalSignature ds;
                ds.GetContents(instruction);
                DigitalSignatureValue.push_back(ds);
            }
            else if (!strcmp(instructionName, "S100XC:CompressionFlag"))
            {
                CompressionFlag = ExXmlSupport().ParseStr2Bool(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:DefaultLocale"))
            {
                PT_Locale loc;
                loc.GetContents(instruction);
                DefaultLocale = std::make_shared<PT_Locale>(loc);
            }
            else if (!strcmp(instructionName, "S100XC:OtherLocale"))
            {
                PT_Locale loc;
                loc.GetContents(instruction);
                OtherLocale.push_back(loc);
            }
            else
            {
            }
        }
    }

    void CatalogueDiscoveryMetadata::SaveXmlNode(pugi::xml_node& node)
    {
        {
            auto child= node.append_child("S100XC:fileName");
            child.text().set(FileName.c_str());
        }
        if (purpose)
        {
            auto child = node.append_child("S100XC:Purpose");
            child.text().set(S100_PurposeToString(*purpose).c_str());
        }
        {
            auto child = node.append_child("S100XC:EditionNumber");
            child.text().set(std::to_string(EditionNumber).c_str());
        }
        {
            auto child = node.append_child("S100XC:Scope");
            child.text().set(S100_CatalogueScopeToString(Scope).c_str());
        }
        if (!VersionNumber.empty())
        {
            auto child = node.append_child("S100XC:VersionNumber");
            child.text().set(VersionNumber.c_str());
        }
        {
            auto child = node.append_child("S100XC:IssueDate");
            child.text().set(IssueDate.ToString().c_str());
        }
        {
            auto child = node.append_child("S100XC:ProductSpecification");
            productSpecification.Save(child);
        }
        {
            auto child = node.append_child("S100XC:DigitalSignatureReference");
            child.text().set(S100_SE_DigitalSignatureReferenceToString(DigitalSignatureReference).c_str());
        }
        if (!DigitalSignatureValue.empty())
        {
            for (int i = 0; i < DigitalSignatureValue.size(); i++)
            {
                auto child = node.append_child("S100XC:DigitalSignatureValue");
                DigitalSignatureValue[i].Save(child);
            }
        }
        {
            auto child = node.append_child("S100XC:CompressionFlag");
            child.text().set(ExXmlSupport().ParseBool2Str(CompressionFlag).c_str());
        }
        if (DefaultLocale)
        {
            auto child = node.append_child("S100XC:DefaultLocale");
            DefaultLocale->Save(child);
        }
        if (!OtherLocale.empty())
        {
            for (int i = 0 ; i< OtherLocale.size() ; i++)
            {
                auto child = node.append_child("S100XC:OtherLocale");
                OtherLocale[i].Save(child);
            }
        }
    }
}
