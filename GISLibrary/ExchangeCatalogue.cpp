#include "stdafx.h"
#include "../GeoMetryLibrary/TextCodec.h"
#include "ExchangeCatalogue.h"
#include "GISLibrary.h"
#include "S10XGML.h"
#include "ExchangeCatalogueNameSpace.h"

#include "../LatLonUtility/LatLonUtility.h"
#include "../LatLonUtility/cpp_util.h"


#include <filesystem>

namespace S100
{
    ExchangeCatalogue::ExchangeCatalogue()
    {
    }

    ExchangeCatalogue::~ExchangeCatalogue()
    {
    }

    void ExchangeCatalogue::Open(std::string filePath)
    {
        (void)OpenFile(std::filesystem::path(filePath));
    }

    bool ExchangeCatalogue::OpenFile(const std::filesystem::path& filePath)
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(filePath.c_str());
        if (!result)
        {
            return false;
        }
        pugi::xml_node nodeList = doc.first_child();

        const pugi::char_t* instructionName = nodeList.name();
        if (!strcmp(instructionName, "S100XC:S100_ExchangeCatalogue") || !strcmp(instructionName, "S100XC:S-101_ExchangeCatalogue"))
        {
            GetContents(nodeList);
            return true;
        }
        return false;
    }

    bool ExchangeCatalogue::Save(std::string filePath)
    {
        std::filesystem::path pathObj(filePath);
        std::filesystem::path dirPath = pathObj.parent_path();

        if (!std::filesystem::exists(dirPath))
            return false;

        if (std::filesystem::exists(filePath)) 
            return false;

        pugi::xml_document doc;
        //doc.append_attribute("encoding").set_value("UTF-8");

        auto child = doc.append_child("S100XC:S100_ExchangeCatalogue");

        for (auto item : ExchangeCatalogueNameSpaceMap)
            child.append_attribute(item.first.c_str()).set_value(item.second.c_str());


        if (Identifier)
        {
            auto item = child.append_child("S100XC:identifier");
            Identifier->Save(item);
        }
        if (Contact)
        {
            auto item = child.append_child("S100XC:contact");
            Contact->Save(item);
        }
        if (productSpecification)
        {
            auto item = child.append_child("S100XC:productSpecification");
            productSpecification->Save(item);
        }
        if (DefaultLocale)
        {
            auto item = child.append_child("S100XC:defaultLocale");
            DefaultLocale->Save(item);
        }
        if (!OtherLocale.empty())
        {
            auto temp = child.append_child("S100XC:otherLocale");
            for (int i = 0; i < OtherLocale.size(); i++)
                OtherLocale[i].Save(temp);
        }
        if (ExchangeCatalogueDescription)
        {
            auto item = child.append_child("S100XC:exchangeCatalogueDescription");
            item.text().set(ExchangeCatalogueDescription->c_str());
        }
        if (ExchangeCatalogueComment)
        {
            auto item = child.append_child("S100XC:exchangeCatalogueComment");
            item.text().set(ExchangeCatalogueComment->c_str());
        }
        if (!Certificates.empty())
        {
            auto temp = child.append_child("S100XC:certificates");
            for (int i = 0; i < Certificates.size(); i++)
                Certificates[i].Save(temp);
        }
        if (DataServerIdentifier)
        {
            auto item = child.append_child("S100XC:dataServerIdentifier");
            item.text().set(DataServerIdentifier->c_str());
        }
        if (DatasetDiscoveryMetadata.size() != 0)
        {
            auto item = child.append_child("S100XC:datasetDiscoveryMetadata");
            DatasetDiscoveryMetadata.SaveXmlNode(item,"S100XC:S100_DatasetDiscoveryMetadata");
        }
        if (CatalogueDiscoveryMetadata.size() != 0)
        {
            auto item = child.append_child("S100XC:catalogueDiscoveryMetadata");
            CatalogueDiscoveryMetadata.SaveXmlNode(item, "S100XC:S100_CatalogueDiscoveryMetadata");
        }
        if (SupportFileDiscoveryMetadata.size() != 0)
        {
            auto item = child.append_child("S100XC:supportFileDiscoveryMetadata");
            SupportFileDiscoveryMetadata.SaveXmlNode(item, "S100XC:S100_SupportFileDiscoveryMetadata");
        }

        bool saveSucceeded = doc.save_file(filePath.c_str(), "\t", pugi::format_default, pugi::encoding_utf8);
        if (saveSucceeded) {
        }
        else {
        }

     /*   if (!doc.save_file(filePath.c_str())) {
            return false;
        }*/

        return true;
    }

    void ExchangeCatalogue::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "S100XC:identifier"))
            {
                ExchangeCatalogueIdentifier ec;
                ec.GetContents(instruction);
                Identifier = std::make_shared<ExchangeCatalogueIdentifier>(ec);
            }
            else if (!strcmp(instructionName, "S100XC:contact"))
            {
                CataloguePointOfContact cp;
                cp.GetContents(instruction);
                Contact = std::make_shared<CataloguePointOfContact>(cp);
            }
            else if (!strcmp(instructionName, "S100XC:productSpecification"))
            {
                ProductSpecification ps;
                ps.GetContents(instruction);
                productSpecification = std::make_shared<ProductSpecification>(ps);
            }
            else if (!strcmp(instructionName, "S100XC:defaultLocale"))
            {
                PT_Locale pt;
                pt.GetContents(instruction);
                DefaultLocale = std::make_shared<PT_Locale>(pt);
            }
            else if (!strcmp(instructionName, "S100XC:otherLocale"))
            {
                PT_Locale pt;
                pt.GetContents(instruction);
                OtherLocale.push_back(pt);
            }
            else if (!strcmp(instructionName, "S100XC:exchangeCatalogueDescription"))
            {
                ExchangeCatalogueDescription = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:exchangeCatalogueComment"))
            {
                ExchangeCatalogueComment = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:certificates"))
            {
                S100_SE_CertificateContainerType cc;
                cc.GetContents(instruction);
                Certificates.push_back(cc);
            }
            else if (!strcmp(instructionName, "S100XC:dataServerIdentifier"))
            {
                DataServerIdentifier = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:datasetDiscoveryMetadata"))
            {
                DatasetDiscoveryMetadata.parseXmlNode(instruction, "S100XC:S100_DatasetDiscoveryMetadata");
            }
            else if (!strcmp(instructionName, "S100XC:catalogueDiscoveryMetadata"))
            {
                CatalogueDiscoveryMetadata.parseXmlNode(instruction, "S100XC:S100_CatalogueDiscoveryMetadata");
            }
            else if (!strcmp(instructionName, "S100XC:supportFileDiscoveryMetadata"))
            {
                SupportFileDiscoveryMetadata.parseXmlNode(instruction, "S100XC:S100_SupportFileDiscoveryMetadata");
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }

}
