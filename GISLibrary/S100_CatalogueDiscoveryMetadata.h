#pragma once
#include "PT_Locale.h"
#include "URI.h"
#include "S100_Purpose.h"
#include "S100_CatalogueScope.h"
#include "XML_Collection.h"

#include <vector>
#include <memory>

namespace S100 {
	class S100_CatalogueDiscoveryMetadata : public xmlParseable
	{
	public:
		URI FileName;
		std::shared_ptr<S100_Purpose> Purpose;
		int EditionNumber;
		S100_CatalogueScope Scope;
		std::string VersionNumber;
		S100_Date IssueDate;
		S100_ProductSpecification ProductSpecification;
		S100_SE_DigitalSignatureReference  DigitalSignatureReference;
		std::vector<S100_SE_DigitalSignature>  DigitalSignatureValue;
		bool CompressionFlag;
		std::shared_ptr<PT_Locale> DefaultLocale;
		std::vector<PT_Locale> OtherLocale;


		inline void GetContents(pugi::xml_node& node) override
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
					Purpose = make_shared<S100_Purpose>(S100_PurposeFromString(instruction.child_value()));
				}
				else if (!strcmp(instructionName, "S100XC:EditionNumber"))
				{
					EditionNumber = atoi(instruction.child_value());
				}
				else if (!strcmp(instructionName, "S100XC:Scope"))
				{
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
					S100_ProductSpecification ps;
					ps.GetContents(instruction);
					ProductSpecification = ps;
				}
				else if (!strcmp(instructionName, "S100XC:DigitalSignatureReference"))
				{
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
					CompressionFlag = ParseStr2Bool(instruction.child_value());
				}
				else if (!strcmp(instructionName, "S100XC:DefaultLocale"))
				{
					PT_Locale loc;
					loc.GetContents(instruction);
					DefaultLocale = make_shared<PT_Locale>(loc);
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
	};
}