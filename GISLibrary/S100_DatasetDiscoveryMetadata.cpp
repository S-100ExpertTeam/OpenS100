#include "stdafx.h"
#include "S100_DatasetDiscoveryMetadata.h"

namespace S100
{
    void S100_DatasetDiscoveryMetadata::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "S100XC:fileName"))
            {
                FileName = instruction.child_value();
            }
            else if (!strcmp(instructionName, "S100XC:description"))
            {
                Description = make_shared<string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:datasetID"))
            {
                DatasetID = make_shared<URN>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:compressionFlag"))
            {
                CompressionFlag = ParseStr2Bool(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:dataProtection"))
            {
                DataProtection = ParseStr2Bool(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:protectionScheme"))
            {
                ProtectionScheme = make_shared<S100_ProtectionScheme>(S100_ProtectionSchemeFromString(instruction.child_value()));
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
            else if (!strcmp(instructionName, "S100XC:copyright"))
            {
                Copyright = ParseStr2Bool(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:classification"))
            {
                Classification = make_shared<MD_ClassificationCode>(MD_ClassificationCodeFromString(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:purpose"))
            {
                Purpose = make_shared<S100_Purpose>(S100_PurposeFromString(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:notForNavigation"))
            {
                NotForNavigation = ParseStr2Bool(instruction.child_value());;
            }
            else if (!strcmp(instructionName, "S100XC:specificUsage"))
            {
                SpecificUsage = make_shared<string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:editionNumber"))
            {
                EditionNumber = make_shared<int>(atoi(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:updateNumber"))
            {
                UpdateNumber = make_shared<int>(atoi(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:updateApplicationDate"))
            {
                UpdateApplicationDate = make_shared<S100_Date>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:referenceID"))
            {
                ReferenceID = make_shared<URN>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:issueDate"))
            {
                IssueDate = S100_Date(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:issueTime"))
            {
                IssueTime = make_shared<S100_Time>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:boundingBox"))
            {
                EX_GeographicBoundingBox gb;
                gb.GetContents(instruction);
                BoundingBox = make_shared<EX_GeographicBoundingBox>(gb);
            }
            else if (!strcmp(instructionName, "S100XC:temporalExtent"))
            {
                S100_TemporalExtent te;
                te.GetContents(instruction);
                TemporalExtent = make_shared<S100_TemporalExtent>(te);
            }
            else if (!strcmp(instructionName, "S100XC:productSpecification"))
            {
                S100_ProductSpecification ps;
                ps.GetContents(instruction);
                ProductSpecification = ps;
            }
            else if (!strcmp(instructionName, "S100XC:producingAgency"))
            {
                CI_Responsibility os;
                auto temp = GetContentNode(instruction, "CI_Responsibility", "cit");
                os.GetContents(temp);
                ProducingAgency = os;
            }
            else if (!strcmp(instructionName, "S100XC:producerCode"))
            {
                ProducerCode = make_shared<string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:encodingFormat"))
            {
                EncodingFormat = S100_EncodingFormatFromString(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:dataCoverage"))
            {
                S100_DataCoverage dc;
                dc.GetContents(instruction);
                DataCoverage.push_back(dc);
            }
            else if (!strcmp(instructionName, "S100XC:comment"))
            {
                Comment = make_shared<string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:defaultLocale"))
            {
                PT_Locale lo;
                lo.GetContents(instruction);
                DefaultLocale = make_shared<PT_Locale>(lo);
            }
            else if (!strcmp(instructionName, "S100XC:otherLocale"))
            {
                PT_Locale lo;
                lo.GetContents(instruction);
                OtherLocale.push_back(lo);
            }
            else if (!strcmp(instructionName, "S100XC:metadataPointOfContact"))
            {
                CI_Responsibility rb;
                rb.GetContents(instruction);
                MetadataPointOfContact = make_shared<CI_Responsibility>(rb);
            }
            else if (!strcmp(instructionName, "S100XC:metadataDateStamp"))
            {
                MetadataDateStamp = make_shared<S100_Date>(S100_Date(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:replacedData"))
            {
                ReplacedData = make_shared<bool>(ParseStr2Bool(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:dataReplacement"))
            {
                DataReplacement.push_back(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:navigationPurpose"))
            {
                NavigationPurpose.push_back(S100_NavigationPurposeFromString(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:resourceMaintenance"))
            {
                MD_MaintenanceInformation mi;
                mi.GetContents(instruction);
                ResourceMaintenance = make_shared<MD_MaintenanceInformation>(mi);
            }
            else
            {
            }
        }
    }
}  
