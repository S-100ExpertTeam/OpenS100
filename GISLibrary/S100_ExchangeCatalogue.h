#pragma once

#include "S100_ExchangeCatalogueIdentifier.h"
#include "S100_ProductSpecification.h"
#include "S100_SE_CertificateContainerType.h"
#include "S100_DatasetDiscoveryMetadata.h"
#include "S100_CatalogueDiscoveryMetadata.h"
#include "S100_SupportFileDiscoveryMetadata.h"
#include "PT_Locale.h"
#include "S100_CataloguePointOfContact.h"
#include "pugiSupportClass.h"

#include <string>
#include <memory>
#include <vector>

namespace S100
{
    class S100_ExchangeCatalogue
    {
    public:
        S100_ExchangeCatalogue();
        ~S100_ExchangeCatalogue();

        void Open(std::string filePath);

        std::shared_ptr<S100_ExchangeCatalogueIdentifier> Identifier;
        std::shared_ptr<S100_CataloguePointofContact> Contact;
        std::vector<S100_ProductSpecification> ProductSpecification;
        std::shared_ptr<PT_Locale> DefaultLocale;
        std::vector<PT_Locale> OtherLocale;
        std::shared_ptr<std::string> ExchangeCatalogueDescription;
        std::shared_ptr<std::string> ExchangeCatalogueComment;
        std::vector<S100_SE_CertificateContainerType> Certificates;
        std::shared_ptr<std::string> DataServerIdentifier;
        XmlCollection<S100_DatasetDiscoveryMetadata> DatasetDiscoveryMetadata;
        XmlCollection<S100_CatalogueDiscoveryMetadata> CatalogueDiscoveryMetadata;
        XmlCollection<S100_SupportFileDiscoveryMetadata> SupportFileDiscoveryMetadata;

        void GetContents(pugi::xml_node& node);

        void DrawCoverage(ID2D1RenderTarget* pRenderTarget, ID2D1Factory* pDXFactory, Scaler* scaler, double offsetX = 0, double offsetY = 0);
    };
}
