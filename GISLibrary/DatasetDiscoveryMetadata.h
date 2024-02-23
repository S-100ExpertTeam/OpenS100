#pragma once

#include "MD_MaintenanceInformation.h"
#include "URI.h"
#include "URN.h"
#include "Purpose.h"
#include "ProtectionScheme.h"
#include "MD_ClassificationCode.h"
#include "EX_GeographicBoundingBox.h"
#include "TemporalExtent.h"
#include "EncodingFormat.h"
#include "DataCoverage.h"
#include "CI_Responsibility.h"
#include "NavigationPurpose.h"
#include "S100_Time.h"
#include "PT_Locale.h"
#include "CI_Organisation.h"
#include "S100_SE_DigitalSignatureReference.h"
#include "S100_SE_DigitalSignature.h"
#include "XML_Collection.h"

#include <memory>
#include <vector>
#include <pugixml.hpp>
#include "ProductSpecification.h"

namespace S100
{
    class DatasetDiscoveryMetadata : public xmlParseable
    {
    public:
        URI FileName;
        std::shared_ptr<std::string> Description;
        std::shared_ptr<URN> DatasetID;
        std::shared_ptr<bool> CompressionFlag;
        std::shared_ptr<bool> DataProtection;
        std::shared_ptr<ProtectionScheme> protectionScheme;
        std::shared_ptr<S100_SE_DigitalSignatureReference> DigitalSignatureReference;
        std::vector<S100_SE_DigitalSignature> DigitalSignatureValue;
        std::shared_ptr<bool> Copyright;
        std::shared_ptr<MD_ClassificationCode> Classification;
        std::shared_ptr<Purpose> purpose;
        std::shared_ptr<bool> NotForNavigation;
        std::shared_ptr<std::string> SpecificUsage;
        std::shared_ptr<int> EditionNumber;
        std::shared_ptr<int> UpdateNumber;
        std::shared_ptr<S100_Date> UpdateApplicationDate;
        std::shared_ptr<URN> ReferenceID;
        std::shared_ptr<S100_Date> IssueDate;
        std::shared_ptr<S100_Time> IssueTime;
        std::shared_ptr<EX_GeographicBoundingBox> BoundingBox;
        std::shared_ptr<TemporalExtent> temporalExtent;
        std::shared_ptr<ProductSpecification> productSpecification;
        std::shared_ptr<CI_Responsibility> ProducingAgency;
        std::shared_ptr<std::string> ProducerCode;
        std::shared_ptr<EncodingFormat> EncodingFormat;
        std::vector<DataCoverage>  dataCoverage;
        std::shared_ptr<std::string> Comment;
        std::shared_ptr<PT_Locale> DefaultLocale;
        std::vector<PT_Locale> OtherLocale;
        std::shared_ptr<CI_Responsibility> MetadataPointOfContact;
        std::shared_ptr<S100_Date> MetadataDateStamp;
        std::shared_ptr<bool> ReplacedData;
        std::vector<std::string> DataReplacement;
        std::vector<NavigationPurpose> NavigationPurpose;
        std::shared_ptr<MD_MaintenanceInformation> ResourceMaintenance;

        void GetContents(pugi::xml_node& node) override;
        void SaveXmlNode(pugi::xml_node& node) override;
    };

}
