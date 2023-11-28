#pragma once

#include "PT_Locale.h"
#include "URI.h"
#include "S100_Purpose.h"
#include "S100_CatalogueScope.h"
#include "XML_Collection.h"
#include "S100_Date.h"
#include "S100_ProductSpecification.h"
#include "S100_SE_DigitalSignatureReference.h"
#include "S100_SE_DigitalSignature.h"

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

        void GetContents(pugi::xml_node& node) override;
    };
}
