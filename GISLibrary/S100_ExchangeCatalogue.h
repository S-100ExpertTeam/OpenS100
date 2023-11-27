#pragma once

#include "S100_ExchangeCatalogueIdentifier.h"
#include "S100_ProductSpecification.h"
#include "S100_SE_CertificateContainerType.h"
#include "S100_DatasetDiscoveryMetadata.h"
#include "S100_CatalogueDiscoveryMetadata.h"
#include "S100_SupportFileDiscoveryMetadata.h"
#include "PT_Locale.h"
#include "S100_CataloguePointOfContact.h"
#include "pugiSupportClass.h"

#include <string>
#include <memory>
#include <vector>

namespace S100 
{
	class S100_ExchangeCatalogue
	{
	public:
		std::shared_ptr<S100_ExchangeCatalogueIdentifier> Identifier;
		std::shared_ptr<S100_CataloguePointofContact> Contact;
		std::vector<S100_ProductSpecification>  ProductSpecification;
		std::shared_ptr<PT_Locale> DefaultLocale;
		std::vector<PT_Locale> OtherLocale;
		std::shared_ptr<std::string> ExchangeCatalogueDescription;
		std::shared_ptr<std::string> ExchangeCatalogueComment;
		std::vector<S100_SE_CertificateContainerType> Certificates;
		std::shared_ptr<std::string> DataServerIdentifier;
		XmlCollection<S100_DatasetDiscoveryMetadata>  DatasetDiscoveryMetadata;
		XmlCollection<S100_CatalogueDiscoveryMetadata> CatalogueDiscoveryMetadata;
		XmlCollection<S100_SupportFileDiscoveryMetadata> SupportFileDiscoveryMetadata;

		inline void Open(std::string filePath)
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(filePath.c_str());
			pugi::xml_node nodeList = doc.first_child();

			const pugi::char_t* instructionName = nodeList.name();
			if (!strcmp(instructionName, "S100XC:S100_ExchangeCatalogue"))
			{
				GetContents(nodeList);
			}
			else
			{

			}
		}

		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "S100XC:identifier"))
				{
					S100_ExchangeCatalogueIdentifier ec;
					ec.GetContents(instruction);
					Identifier = std::make_shared< S100_ExchangeCatalogueIdentifier>(ec);
				}
				else if (!strcmp(instructionName, "S100XC:contact"))
				{
					S100_CataloguePointofContact cp;
					cp.GetContents(instruction);
					Contact = std::make_shared<S100_CataloguePointofContact>(cp);
				}
				else if (!strcmp(instructionName, "S100XC:productSpecification"))
				{
					S100_ProductSpecification ps;
					ps.GetContents(instruction);
					ProductSpecification.push_back(ps);
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
					DatasetDiscoveryMetadata.parseXmlNode(instruction,"S100XC:S100_DatasetDiscoveryMetadata");
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

	public:
		int getCountDatasetDiscoveryMetadata();
		std::string getDatasetPath(int index);
	};
}