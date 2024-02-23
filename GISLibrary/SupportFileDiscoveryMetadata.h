#pragma once

#include "PT_Locale.h"
#include "URI.h"
#include "SupportFileFormat.h"
#include "ResourcePurpose.h"
#include "SupportFileRevisionStatus.h"
#include "SupportFileSpecification.h"
#include "XML_Collection.h"
#include <memory>
#include <vector>
#include <string>
#include "pugiSupportClass.h"
#include "S100_SE_DigitalSignatureReference.h"
#include "S100_SE_DigitalSignature.h"

namespace S100 {
    class SupportFileDiscoveryMetadata : public xmlParseable
    {
    public:
        URI FileName;
        std::shared_ptr<SupportFileRevisionStatus> RevisionStatus;
        std::shared_ptr<int> EditionNumber;
        std::shared_ptr<S100_Date> IssueDate;
        std::shared_ptr<SupportFileSpecification> supportFileSpecification;
        std::shared_ptr<SupportFileFormat> DataType;
        std::shared_ptr<std::string> OtherDataTypeDescription;
        std::shared_ptr<std::string> Comment;
        std::shared_ptr<bool> CompressionFlag;
        std::shared_ptr<S100_SE_DigitalSignatureReference> DigitalSignatureReference;
        std::vector<S100_SE_DigitalSignature> DigitalSignatureValue;
        std::shared_ptr<PT_Locale> DefaultLocale;
        std::vector<std::string> SupportedResource;
        std::shared_ptr<ResourcePurpose>  resourcePurpose;

        void GetContents(pugi::xml_node& node) override;
        void SaveXmlNode(pugi::xml_node& node) override;
    };
}
