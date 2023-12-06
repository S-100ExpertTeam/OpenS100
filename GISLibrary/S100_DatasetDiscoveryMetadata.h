#pragma once

#include "MD_MaintenanceInformation.h"
#include "URI.h"
#include "URN.h"
#include "S100_Purpose.h"
#include "S100_ProtectionScheme.h"
#include "MD_ClassificationCode.h"
#include "EX_GeographicBoundingBox.h"
#include "S100_TemporalExtent.h"
#include "S100_EncodingFormat.h"
#include "S100_DataCoverage.h"
#include "CI_Responsibility.h"
#include "S100_NavigationPurpose.h"
#include "S100_Time.h"
#include "PT_Locale.h"
#include "CI_Organisation.h"
#include "S100_SE_DigitalSignatureReference.h"
#include "S100_SE_DigitalSignature.h"
#include "XML_Collection.h"

#include <memory>
#include <vector>
#include <pugixml.hpp>
#include "S100_ProductSpecification.h"

namespace S100
{
    class S100_DatasetDiscoveryMetadata : public xmlParseable
    {
    public:
        URI FileName;
        std::shared_ptr<std::string> Description;
        std::shared_ptr<URN> DatasetID;
        bool CompressionFlag;
        bool DataProtection;
        std::shared_ptr<S100_ProtectionScheme> ProtectionScheme;
        S100_SE_DigitalSignatureReference DigitalSignatureReference;
        std::vector<S100_SE_DigitalSignature> DigitalSignatureValue;
        bool Copyright;
        std::shared_ptr<MD_ClassificationCode> Classification;
        std::shared_ptr<S100_Purpose> Purpose;
        bool NotForNavigation;
        std::shared_ptr<std::string> SpecificUsage;
        std::shared_ptr<int> EditionNumber;
        std::shared_ptr<int> UpdateNumber;
        std::shared_ptr<S100_Date> UpdateApplicationDate;
        std::shared_ptr<URN> ReferenceID;
        S100_Date IssueDate;
        std::shared_ptr<S100_Time> IssueTime;
        std::shared_ptr<EX_GeographicBoundingBox> BoundingBox;
        std::shared_ptr<S100_TemporalExtent> TemporalExtent;
        S100_ProductSpecification ProductSpecification;
        CI_Responsibility ProducingAgency;
        std::shared_ptr<std::string> ProducerCode;
        S100_EncodingFormat EncodingFormat;
        std::vector<S100_DataCoverage>  DataCoverage;
        std::shared_ptr<std::string> Comment;
        std::shared_ptr<PT_Locale> DefaultLocale;
        std::vector<PT_Locale> OtherLocale;
        std::shared_ptr<CI_Responsibility> MetadataPointOfContact;
        std::shared_ptr<S100_Date> MetadataDateStamp;
        std::shared_ptr<bool> ReplacedData;
        std::vector<std::string> DataReplacement;
        std::vector<S100_NavigationPurpose> NavigationPurpose;
        std::shared_ptr<MD_MaintenanceInformation> ResourceMaintenance;

        void GetContents(pugi::xml_node& node) override;
    };

}
