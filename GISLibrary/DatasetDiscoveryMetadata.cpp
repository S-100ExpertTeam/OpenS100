#include "stdafx.h"
#include "DatasetDiscoveryMetadata.h"
#include "S100_SE_SignatureOnData.h"

namespace S100
{
    void DatasetDiscoveryMetadata::GetContents(pugi::xml_node& node)
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
                CompressionFlag = ExXmlSupport().ParseStr2Bool(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:dataProtection"))
            {
                DataProtection = ExXmlSupport().ParseStr2Bool(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:protectionScheme"))
            {
                protectionScheme = make_shared<ProtectionScheme>(S100_ProtectionSchemeFromString(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:digitalSignatureReference"))
            {
                DigitalSignatureReference = make_shared<S100_SE_DigitalSignatureReference>(S100_SE_DigitalSignatureReferenceFromString(instruction.child_value()));
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
            else if (!strcmp(instructionName, "S100XC:copyright"))
            {
                Copyright = ExXmlSupport().ParseStr2Bool(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:classification"))
            {
                if(!string(instruction.child_value()).empty())
                    Classification = make_shared<MD_ClassificationCode>(MD_ClassificationCodeFromString(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:purpose"))
            {
                if (!string(instruction.child_value()).empty())
                    purpose = make_shared<Purpose>(S100_PurposeFromString(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:notForNavigation"))
            {
                NotForNavigation = ExXmlSupport().ParseStr2Bool(instruction.child_value());;
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
                TemporalExtent te;
                te.GetContents(instruction);
                temporalExtent = make_shared<TemporalExtent>(te);
            }
            else if (!strcmp(instructionName, "S100XC:productSpecification"))
            {
                ProductSpecification ps;
                ps.GetContents(instruction);
                productSpecification = ps;
            }
            else if (!strcmp(instructionName, "S100XC:producingAgency"))
            {
                CI_Responsibility os;
                auto temp = ExXmlSupport().GetContentNode(instruction, "CI_Responsibility", "cit");
                os.GetContents(temp);
                ProducingAgency = os;
            }
            else if (!strcmp(instructionName, "S100XC:producerCode"))
            {
                ProducerCode = make_shared<string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:encodingFormat"))
            {
                if (!string(instruction.child_value()).empty())
                    EncodingFormat = S100_EncodingFormatFromString(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:dataCoverage"))
            {
                DataCoverage dc;
                dc.GetContents(instruction);
                dataCoverage.push_back(dc);
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
                ReplacedData = make_shared<bool>(ExXmlSupport().ParseStr2Bool(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "S100XC:dataReplacement"))
            {
                DataReplacement.push_back(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:navigationPurpose"))
            {
                if (!string(instruction.child_value()).empty())
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

    void DatasetDiscoveryMetadata::SaveXmlNode(pugi::xml_node& node)
    {
            if (!FileName.empty())
            {
                auto child = node.append_child("S100XC:fileName");
                child.text().set(FileName.c_str());
            }
            if (Description)
            {
                auto child = node.append_child("S100XC:description");
                child.text().set(Description->c_str());
            }
            if (DatasetID)
            {
                auto child = node.append_child("S100XC:datasetID");
                child.text().set(DatasetID->c_str());
            }
            auto CompressionFlagNode = node.append_child("S100XC:compressionFlag");
            CompressionFlagNode.text().set(ExXmlSupport().ParseBool2Str(CompressionFlag).c_str());

            auto DataProtectionNode = node.append_child("S100XC:dataProtection");
            DataProtectionNode.text().set(ExXmlSupport().ParseBool2Str(DataProtection).c_str());
            if (protectionScheme)
            {
                auto child = node.append_child("S100XC:protectionScheme");
                child.text().set(S100_ProtectionSchemeToString(*protectionScheme).c_str());
            }
            if (DigitalSignatureReference)
            {
                auto child = node.append_child("S100XC:digitalSignatureReference");
                child.text().set(S100_SE_DigitalSignatureReferenceToString(*DigitalSignatureReference).c_str());
            }
            if (!DigitalSignatureValue.empty())
            {
                for (int i = 0; i < DigitalSignatureValue.size(); i++)
                {
                    auto child = node.append_child("S100XC:digitalSignatureValue");
                    DigitalSignatureValue[i].Save(child);
                }
            }
            auto copyrightNode = node.append_child("S100XC:copyright");
            copyrightNode.text().set(ExXmlSupport().ParseBool2Str(Copyright).c_str());

            if (Classification)
            {
                auto child = node.append_child("S100XC:classification");
                child.text().set(MD_ClassificationCodeToString(*Classification).c_str());
            }
            if (purpose)
            {
                auto child = node.append_child("S100XC:purpose");
                child.text().set(S100_PurposeToString(*purpose).c_str());
            }
            auto notForNavigationNode = node.append_child("S100XC:notForNavigation");
            notForNavigationNode.text().set(ExXmlSupport().ParseBool2Str(NotForNavigation).c_str());
            if (SpecificUsage)
            {
                auto child = node.append_child("S100XC:specificUsage");
                child.text().set(SpecificUsage->c_str());
            }
            if (EditionNumber)
            {
                auto child = node.append_child("S100XC:editionNumber");
                child.text().set(std::to_string(*EditionNumber).c_str());
            }
            if (UpdateNumber)
            {
                auto child = node.append_child("S100XC:updateNumber");
                child.text().set(std::to_string(*UpdateNumber).c_str());
            }
            if (UpdateApplicationDate)
            {
                auto child = node.append_child("S100XC:updateApplicationDate");
                child.text().set(UpdateApplicationDate->ToString().c_str());
            }
            if (ReferenceID)
            {
                auto child = node.append_child("S100XC:specificUsage");
                child.text().set(ReferenceID->c_str());
            }
            auto issueDateNode = node.append_child("S100XC:issueDate");
            issueDateNode.text().set(IssueDate.ToString().c_str());
            if (IssueTime)
            {
                auto child = node.append_child("S100XC:issueTime");
                child.text().set(IssueTime->ToString().c_str());
            }
            if (BoundingBox)
            {
                auto child = node.append_child("S100XC:boundingBox");
                BoundingBox->Save(child);
            }
            if (temporalExtent)
            {
                auto child = node.append_child("S100XC:temporalExtent");
                temporalExtent->Save(child);
            }
            {
                auto child = node.append_child("S100XC:productSpecification");
                productSpecification.Save(child);
            }
            {
                auto child = node.append_child("S100XC:producingAgency");
                ProducingAgency.Save(child);
            }
            if (ProducerCode)
            {
                auto child = node.append_child("S100XC:producerCode");
                child.text().set(ProducerCode->c_str());
            }
            {
                auto child = node.append_child("S100XC:encodingFormat");
                child.text().set(S100_EncodingFormatToString(EncodingFormat).c_str());
            }
            if (!dataCoverage.empty())
            {
                for (int i = 0; i < dataCoverage.size(); i++)
                {
                    auto child = node.append_child("S100XC:dataCoverage");
                    dataCoverage[i].Save(child);
                }
            }
            if (Comment)
            {
                auto child = node.append_child("S100XC:comment");
                child.text().set(Comment->c_str());
            }
            if (DefaultLocale)
            {
                auto child = node.append_child("S100XC:defaultLocale");
                DefaultLocale->Save(child);
            }
            if (!OtherLocale.empty())
            {
                for (int i = 0; i < OtherLocale.size(); i++)
                {
                    auto child = node.append_child("S100XC:otherLocale");
                    OtherLocale[i].Save(child);

                }
            }
            if (MetadataPointOfContact)
            {
                auto child = node.append_child("S100XC:metadataPointOfContact");
                MetadataPointOfContact->Save(child);
            }
            if (MetadataDateStamp)
            {
                auto child = node.append_child("S100XC:metadataDateStamp");
                child.text().set(MetadataDateStamp->ToString().c_str());
            }
            if (ReplacedData)
            {
                auto child = node.append_child("S100XC:replacedData");
                child.text().set(ExXmlSupport().ParseBool2Str(*ReplacedData).c_str());
            }
            if (!DataReplacement.empty())
            {
                for (int i = 0; i < DataReplacement.size(); i++)
                {
                    auto child = node.append_child("S100XC:dataReplacement");
                    child.text().set(DataReplacement[i].c_str());
                }
            }
            if (!NavigationPurpose.empty())
            {
                for (int i = 0; i < NavigationPurpose.size(); i++)
                {
                    auto child = node.append_child("S100XC:navigationPurpose");
                    child.text().set(S100_NavigationPurposeToString(NavigationPurpose[i]).c_str());
                }
            }
            if (ResourceMaintenance)
            {
                auto child = node.append_child("S100XC:resourceMaintenance");
                ResourceMaintenance->Save(child);
            }
        
    }
}
