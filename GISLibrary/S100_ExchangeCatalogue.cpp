#include "stdafx.h"
#include "S100_ExchangeCatalogue.h"
#include "GISLibrary.h"
#include "../LatLonUtility/LatLonUtility.h"
#include "../LibMFCUtil/LibMFCUtil.h"
#include "S10XGML.h"

namespace S100
{
    S100_ExchangeCatalogue::S100_ExchangeCatalogue()
    {
    }

    S100_ExchangeCatalogue::~S100_ExchangeCatalogue()
    {
    }

    void S100_ExchangeCatalogue::Open(std::string filePath)
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(filePath.c_str());
        pugi::xml_node nodeList = doc.first_child();

        const pugi::char_t* instructionName = nodeList.name();
        if (!strcmp(instructionName, "S100XC:S100_ExchangeCatalogue"))
        {
            GetContents(nodeList);
        }
        else
        {

        }
    }

    void S100_ExchangeCatalogue::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "S100XC:identifier"))
            {
                S100_ExchangeCatalogueIdentifier ec;
                ec.GetContents(instruction);
                Identifier = std::make_shared<S100_ExchangeCatalogueIdentifier>(ec);
            }
            else if (!strcmp(instructionName, "S100XC:contact"))
            {
                S100_CataloguePointofContact cp;
                cp.GetContents(instruction);
                Contact = std::make_shared<S100_CataloguePointofContact>(cp);
            }
            else if (!strcmp(instructionName, "S100XC:productSpecification"))
            {
                S100_ProductSpecification ps;
                ps.GetContents(instruction);
                ProductSpecification.push_back(ps);
            }
            else if (!strcmp(instructionName, "S100XC:defaultLocale"))
            {
                PT_Locale pt;
                pt.GetContents(instruction);
                DefaultLocale = std::make_shared<PT_Locale>(pt);
            }
            else if (!strcmp(instructionName, "S100XC:otherLocale"))
            {
                PT_Locale pt;
                pt.GetContents(instruction);
                OtherLocale.push_back(pt);
            }
            else if (!strcmp(instructionName, "S100XC:exchangeCatalogueDescription"))
            {
                ExchangeCatalogueDescription = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:exchangeCatalogueComment"))
            {
                ExchangeCatalogueComment = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:certificates"))
            {
                S100_SE_CertificateContainerType cc;
                cc.GetContents(instruction);
                Certificates.push_back(cc);
            }
            else if (!strcmp(instructionName, "S100XC:dataServerIdentifier"))
            {
                DataServerIdentifier = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "S100XC:datasetDiscoveryMetadata"))
            {
                DatasetDiscoveryMetadata.parseXmlNode(instruction, "S100XC:S100_DatasetDiscoveryMetadata");
            }
            else if (!strcmp(instructionName, "S100XC:catalogueDiscoveryMetadata"))
            {
                CatalogueDiscoveryMetadata.parseXmlNode(instruction, "S100XC:S100_CatalogueDiscoveryMetadata");
            }
            else if (!strcmp(instructionName, "S100XC:supportFileDiscoveryMetadata"))
            {
                SupportFileDiscoveryMetadata.parseXmlNode(instruction, "S100XC:S100_SupportFileDiscoveryMetadata");
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }

    void S100_ExchangeCatalogue::DrawCoverage(ID2D1RenderTarget* pRenderTarget, ID2D1Factory* pDXFactory, Scaler* scaler, double offsetX, double offsetY)
    {
        double Width = gisLib->GetScaler()->GetScreenWidth();
        double Height = gisLib->GetScaler()->GetScreenWidth();

        gisLib->D2.pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        gisLib->D2.pDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

        for (int i = 0; i < DatasetDiscoveryMetadata.size(); i++)
        {
            pRenderTarget->BeginDraw();
            auto textformat = gisLib->D2.pDWriteTextFormat;
            auto brush = gisLib->D2.pBrush;
            auto ddm = DatasetDiscoveryMetadata[i];

            auto object = new GM::Surface();
            auto curve = new GM::Curve();

            auto bb = ddm.BoundingBox;

            double lon = bb->WestBoundLongitude;
            double lat = bb->NorthBoundLatitude;
            curve->Add(lon, lat);

            lon = bb->EastBoundLongitude;
            lat = bb->NorthBoundLatitude;
            curve->Add(lon, lat);

            lon = bb->EastBoundLongitude;
            lat = bb->SouthBoundLatitude;
            curve->Add(lon, lat);

            lon = bb->WestBoundLongitude;
            lat = bb->SouthBoundLatitude;
            curve->Add(lon, lat);
     
            object->SetExteriorRing(curve);

            S10XGML* cls = new S10XGML();
            auto sSurface = cls->SurfaceToSSurface(object);
            sSurface->CreateD2Geometry(gisLib->D2.Factory());
            brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
            pRenderTarget->SetTransform(scaler->GetMatrix());
            pRenderTarget->DrawGeometry(sSurface->GetD2Geometry(), brush, 1.0, gisLib->D2.SolidStrokeStyle());

            delete object;
            delete cls;

            D2D1_POINT_2F pt;
            pt.x = bb->WestBoundLongitude;
            pt.y = bb->NorthBoundLatitude;

            projection(pt.x,pt.y);

            gisLib->WorldToDevice(pt);

            CString cstrFileName = CString(ddm.FileName.c_str());
            CString fileName = L"FileName : " + cstrFileName;
            brush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
            pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  
            pRenderTarget->DrawTextW(fileName, fileName.GetLength(), textformat, D2D1::RectF(pt.x, pt.y, pt.x + 5, pt.y + 5), brush);

            //Datacorverage
            for (int j = 0; j < ddm.DataCoverage.size(); j++)
            {
                gisLib->D2.pRT->SetTransform(scaler->GetMatrix());

                 auto dc = ddm.DataCoverage[j]; 
                 auto object = new GM::Surface();
               
                 for (int geoms = 0; geoms < dc.BoundingPolygon.Polygon.Geom.size(); geoms++)
                 {
                     auto curve = new GM::Curve();
                     
                     auto strPosList = LatLonUtility::Split(LatLonUtility::TrimRight(dc.BoundingPolygon.Polygon.Geom[geoms]), " ");
                     int posCnt = strPosList.size();

                     if (posCnt < 4 && posCnt % 2 != 0)
                         break;

                     for (int i = 0; i < posCnt; i += 2)
                     {
                         double lon = std::stod(strPosList.at(i));
                         double lat = std::stod(strPosList.at(i + 1));
                         curve->Add(lon, lat);
                     }
                     object->SetExteriorRing(curve);
                 }

                 S10XGML* cls = new S10XGML();
                 auto sSurface = cls->SurfaceToSSurface(object);
                 sSurface->CreateD2Geometry(gisLib->D2.Factory());
                 brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
                 brush->SetOpacity(0.3);
                 pRenderTarget->FillGeometry(sSurface->GetD2Geometry(), brush);

                 brush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
                 pRenderTarget->DrawGeometry(sSurface->GetD2Geometry(), brush, 1.0, gisLib->D2.SolidStrokeStyle());

                 delete object;
                 delete cls;

                 pt.y += 15;
                 ////maximumDisplayScale 
                 CString maximumDisplayScale;
                 if (dc.MaximumDisplayScale != nullptr)
                 {
                     CString str(std::to_string(*dc.MaximumDisplayScale).c_str());
                     maximumDisplayScale = L"MaximumDisplayScale : " + str;
                 }
                 else
                     maximumDisplayScale = L"MaximumDisplayScale : - ";
                 brush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
                 brush->SetOpacity(1);
                 pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
                 pRenderTarget->DrawTextW(maximumDisplayScale, maximumDisplayScale.GetLength(), textformat, D2D1::RectF(pt.x, pt.y, pt.x + 5, pt.y + 5), brush);
                 
                 pt.y += 15;
                ////minimumDisplayScale 
                CString minimumDisplayScale;
                if (dc.MinimumDisplayScale != nullptr)
                {
                    CString str(std::to_string(*dc.MinimumDisplayScale).c_str());
                    minimumDisplayScale = L"MinimumDisplayScale : " + str;
                }
                else
                    minimumDisplayScale = L"MinimumDisplayScale : - ";
                brush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
                pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
                pRenderTarget->DrawTextW(minimumDisplayScale, minimumDisplayScale.GetLength(), textformat, D2D1::RectF(pt.x, pt.y, pt.x + 5, pt.y + 5), brush);

                pt.y += 15;
            }

            pRenderTarget->EndDraw();
        }
    }
}
