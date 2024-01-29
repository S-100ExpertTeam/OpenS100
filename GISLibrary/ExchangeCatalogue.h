#pragma once

#include "ExchangeCatalogueIdentifier.h"
#include "ProductSpecification.h"
#include "S100_SE_CertificateContainerType.h"
#include "DatasetDiscoveryMetadata.h"
#include "CatalogueDiscoveryMetadata.h"
#include "SupportFileDiscoveryMetadata.h"
#include "PT_Locale.h"
#include "CataloguePointOfContact.h"
#include "pugiSupportClass.h"

#include <string>
#include <memory>
#include <vector>

namespace S100
{
    class ExchangeCatalogue
    {
    public:
        ExchangeCatalogue();
        ~ExchangeCatalogue();
        void Open(std::string filePath);
        bool Save(std::string filePath);

        std::shared_ptr<ExchangeCatalogueIdentifier> Identifier;
        std::shared_ptr<CataloguePointOfContact> Contact;
        std::shared_ptr<ProductSpecification> productSpecification;
        std::shared_ptr<PT_Locale> DefaultLocale;
        std::vector<PT_Locale> OtherLocale;
        std::shared_ptr<std::string> ExchangeCatalogueDescription;
        std::shared_ptr<std::string> ExchangeCatalogueComment;
        std::vector<S100_SE_CertificateContainerType> Certificates;
        std::shared_ptr<std::string> DataServerIdentifier;
        XmlCollection<DatasetDiscoveryMetadata> DatasetDiscoveryMetadata;
        XmlCollection<CatalogueDiscoveryMetadata> CatalogueDiscoveryMetadata;
        XmlCollection<SupportFileDiscoveryMetadata> SupportFileDiscoveryMetadata;

        void GetContents(pugi::xml_node& node);

        void DrawCoverage(D2D1Resources* pD2, Scaler* scaler, double offsetX = 0, double offsetY = 0);
    };
}
