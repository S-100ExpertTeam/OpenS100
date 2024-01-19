#pragma once

#include "PT_Locale.h"
#include "URI.h"
#include "Purpose.h"
#include "CatalogueScope.h"
#include "XML_Collection.h"
#include "S100_Date.h"
#include "ProductSpecification.h"
#include "S100_SE_DigitalSignatureReference.h"
#include "S100_SE_DigitalSignature.h"

#include <vector>
#include <memory>


namespace S100 {
    class CatalogueDiscoveryMetadata : public xmlParseable
    {
    public:
        URI FileName;
        std::shared_ptr<Purpose> purpose;
        int EditionNumber;
        CatalogueScope Scope;
        std::string VersionNumber;
        S100_Date IssueDate;
        ProductSpecification productSpecification;
        S100_SE_DigitalSignatureReference  DigitalSignatureReference;
        std::vector<S100_SE_DigitalSignature>  DigitalSignatureValue;
        bool CompressionFlag;
        std::shared_ptr<PT_Locale> DefaultLocale;
        std::vector<PT_Locale> OtherLocale;

        void GetContents(pugi::xml_node& node) override;
        void SaveXmlNode(pugi::xml_node& node) override;
    };
}
