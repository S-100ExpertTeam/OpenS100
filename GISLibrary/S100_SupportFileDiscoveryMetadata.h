#pragma once
#include "PT_Locale.h"
#include "URI.h"
#include "S100_SupportFileFormat.h"
#include "S100_ResourcePurpose.h"
#include "S100_SupportFileRevisionStatus.h"
#include "S100_SupportFileSpecification.h"
#include "XML_Collection.h"

#include <memory>
#include <vector>
#include <string>


using namespace std;



namespace S100 {
	class S100_SupportFileDiscoveryMetadata : public xmlParseable
	{
	public:
		URI FileName;
		S100_SupportFileRevisionStatus RevisionStatus;
		int EditionNumber;
		shared_ptr<S100_Date> IssueDate;
		shared_ptr<S100_SupportFileSpecification> SupportFileSpecification;
		S100_SupportFileFormat DataType;
		shared_ptr<string> OtherDataTypeDescription;
		shared_ptr<string> Comment;
		bool CompressionFlag;
		S100_SE_DigitalSignatureReference DigitalSignatureReference;
		vector<S100_SE_DigitalSignature> DigitalSignatureValue;
		shared_ptr<PT_Locale> DefaultLocale;
		vector<string> SupportedResource;
		shared_ptr<S100_ResourcePurpose>  ResourcePurpose;

		inline void GetContents(pugi::xml_node& node) override
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
					IssueDate = make_shared<S100_Date>(S100_Date(instruction.child_value()));
				}
				else if (!strcmp(instructionName, "S100XC:supportFileSpecification"))
				{
					S100_SupportFileSpecification sf;
					sf.GetContents(instruction);
					SupportFileSpecification = make_shared<S100_SupportFileSpecification>(sf);
				}
				else if (!strcmp(instructionName, "S100XC:dataType"))
				{
					DataType = S100_SupportFileFormatFromString(instruction.child_value());
				}
				else if (!strcmp(instructionName, "S100XC:otherDataTypeDescription"))
				{
					OtherDataTypeDescription = make_shared<string>(instruction.child_value());
				}
				else if (!strcmp(instructionName, "S100XC:comment"))
				{
					Comment = make_shared<string>(instruction.child_value());
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
					DefaultLocale = make_shared<PT_Locale>(loc);
				}
				else if (!strcmp(instructionName, "S100XC:supportedResource"))
				{
					SupportedResource.push_back(instruction.child_value());
				}
				else if (!strcmp(instructionName, "S100XC:resourcePurpose"))
				{
					ResourcePurpose = make_shared<S100_ResourcePurpose>(S100_ResourcePurposeFromString(instruction.child_value()));
				}
				else
				{
				}
			}
		}
	};
}