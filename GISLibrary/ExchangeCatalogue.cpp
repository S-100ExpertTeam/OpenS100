#include "stdafx.h"
#include "ExchangeCatalogue.h"
#include "GISLibrary.h"
#include "../LatLonUtility/LatLonUtility.h"
#include "../LibMFCUtil/LibMFCUtil.h"
#include "S10XGML.h"
#include "ExchangeCatalogueNameSpace.h"

#include <filesystem>

namespace S100
{
    ExchangeCatalogue::ExchangeCatalogue()
    {
    }

    ExchangeCatalogue::~ExchangeCatalogue()
    {
    }

    void ExchangeCatalogue::Open(std::string filePath)
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

    bool ExchangeCatalogue::Save(std::string filePath)
    {
        std::filesystem::path pathObj(filePath);
        std::filesystem::path dirPath = pathObj.parent_path();

        //폴더 없으면 반환  
        if (!std::filesystem::exists(dirPath))
            return false;

        //파일 있으면 반환
        if (std::filesystem::exists(filePath)) 
            return false;

        pugi::xml_document doc;
        //doc.append_attribute("encoding").set_value("UTF-8");

        auto child = doc.append_child("S100XC:S100_ExchangeCatalogue");

        for (auto item : ExchangeCatalogueNameSpaceMap)
            child.append_attribute(item.first.c_str()).set_value(item.second.c_str());


        if (Identifier)
        {
            auto item = child.append_child("S100XC:identifier");
            Identifier->Save(item);
        }
        if (Contact)
        {
            auto item = child.append_child("S100XC:contact");
            Contact->Save(item);
        }
        if (productSpecification)
        {
            auto item = child.append_child("S100XC:productSpecification");
            productSpecification->Save(item);
        }
        if (DefaultLocale)
        {
            auto item = child.append_child("S100XC:defaultLocale");
            DefaultLocale->Save(item);
        }
        if (!OtherLocale.empty())
        {
            auto temp = child.append_child("S100XC:otherLocale");
            for (int i = 0; i < OtherLocale.size(); i++)
                OtherLocale[i].Save(temp);
        }
        if (ExchangeCatalogueDescription)
        {
            auto item = child.append_child("S100XC:exchangeCatalogueDescription");
            item.text().set(ExchangeCatalogueDescription->c_str());
        }
        if (ExchangeCatalogueComment)
        {
            auto item = child.append_child("S100XC:exchangeCatalogueComment");
            item.text().set(ExchangeCatalogueComment->c_str());
        }
        if (!Certificates.empty())
        {
            auto temp = child.append_child("S100XC:certificates");
            for (int i = 0; i < Certificates.size(); i++)
                Certificates[i].Save(temp);
        }
        if (DataServerIdentifier)
        {
            auto item = child.append_child("S100XC:dataServerIdentifier");
            item.text().set(DataServerIdentifier->c_str());
        }
        if (DatasetDiscoveryMetadata.size() != 0)
        {
            auto item = child.append_child("S100XC:datasetDiscoveryMetadata");
            DatasetDiscoveryMetadata.SaveXmlNode(item,"S100XC:S100_DatasetDiscoveryMetadata");
        }
        if (CatalogueDiscoveryMetadata.size() != 0)
        {
            auto item = child.append_child("S100XC:catalogueDiscoveryMetadata");
            CatalogueDiscoveryMetadata.SaveXmlNode(item, "S100XC:S100_CatalogueDiscoveryMetadata");
        }
        if (SupportFileDiscoveryMetadata.size() != 0)
        {
            auto item = child.append_child("S100XC:supportFileDiscoveryMetadata");
            SupportFileDiscoveryMetadata.SaveXmlNode(item, "S100XC:S100_SupportFileDiscoveryMetadata");
        }

        // 파일에 XML 문서 저장
        bool saveSucceeded = doc.save_file(filePath.c_str(), "\t", pugi::format_default, pugi::encoding_utf8);
        if (saveSucceeded) {
            //std::cout << "XML 파일이 성공적으로 저장되었습니다." << std::endl;
        }
        else {
            //std::cout << "XML 파일 저장 실패." << std::endl;
        }

     /*   if (!doc.save_file(filePath.c_str())) {
            return false;
        }*/

        return true;
    }

    void ExchangeCatalogue::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "S100XC:identifier"))
            {
                ExchangeCatalogueIdentifier ec;
                ec.GetContents(instruction);
                Identifier = std::make_shared<ExchangeCatalogueIdentifier>(ec);
            }
            else if (!strcmp(instructionName, "S100XC:contact"))
            {
                CataloguePointOfContact cp;
                cp.GetContents(instruction);
                Contact = std::make_shared<CataloguePointOfContact>(cp);
            }
            else if (!strcmp(instructionName, "S100XC:productSpecification"))
            {
                ProductSpecification ps;
                ps.GetContents(instruction);
                productSpecification = std::make_shared<ProductSpecification>(ps);
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

    void ExchangeCatalogue::DrawCoverage(D2D1Resources* pD2, Scaler* scaler, double offsetX, double offsetY)
    {
        double Width = scaler->GetScreenWidth();
        double Height = scaler->GetScreenWidth();

        pD2->pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        pD2->pDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

        for (int i = 0; i < DatasetDiscoveryMetadata.size(); i++)
        {            
            pD2->pRT->BeginDraw();
            auto textformat = pD2->pDWriteTextFormat;
            auto brush = pD2->pBrush;
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

            S10XGML* cls = new S10XGML(pD2);
            auto sSurface = cls->SurfaceToSSurface(object);
            sSurface->CreateD2Geometry(pD2->Factory());
            brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
            pD2->pRT->SetTransform(scaler->GetMatrix());
            pD2->pRT->DrawGeometry(sSurface->GetD2Geometry(), brush, 1.0, pD2->SolidStrokeStyle());

            delete object;
            delete cls;
            delete curve;
            delete sSurface;

            D2D1_POINT_2F pt;
            pt.x = (FLOAT)bb->WestBoundLongitude;
            pt.y = (FLOAT)bb->NorthBoundLatitude;

            projection(pt.x,pt.y);

            long sx = 0;
            long sy = 0;
            scaler->WorldToDevice(pt.x, pt.y, &sx, &sy);

            pt.x = (float)sx;
            pt.y = (float)sy;

            CString cstrFileName = CString(ddm.FileName.c_str());
            CString fileName = L"FileName : " + cstrFileName;
            brush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
            pD2->pRT->SetTransform(D2D1::Matrix3x2F::Identity());
  
            pD2->pRT->DrawTextW(fileName, fileName.GetLength(), textformat, D2D1::RectF(pt.x, pt.y, (FLOAT)Width, (FLOAT)Height), brush);

            //Datacorverage
            for (int j = 0; j < ddm.dataCoverage.size(); j++)
            {
                pD2->pRT->SetTransform(scaler->GetMatrix());

                 auto dc = ddm.dataCoverage[j]; 
                 auto object = new GM::Surface();
               
                 for (int geoms = 0; geoms < dc.BoundingPolygon.Polygon.Geom.size(); geoms++)
                 {
                     auto curve = new GM::Curve();
                     
                     auto strPosList = LatLonUtility::Split(LatLonUtility::TrimRight(dc.BoundingPolygon.Polygon.Geom[geoms]), " ");
                     int posCnt = (int)strPosList.size();

                     if (posCnt < 4 && posCnt % 2 != 0)
                         break;

                     for (int i = 0; i < posCnt; i += 2)
                     {
                         double lon = std::stod(strPosList.at(i));
                         double lat = std::stod(strPosList.at(i + 1));
                         curve->Add(lon, lat);
                     }
                     object->SetExteriorRing(curve);

                     delete curve;
                 }

                 S10XGML* cls = new S10XGML(pD2);
                 auto sSurface = cls->SurfaceToSSurface(object);
                 sSurface->CreateD2Geometry(pD2->Factory());
                 brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
                 brush->SetOpacity((FLOAT)0.3);
                 pD2->pRT->FillGeometry(sSurface->GetD2Geometry(), brush);

                 brush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
                 pD2->pRT->DrawGeometry(sSurface->GetD2Geometry(), brush, 1.0, pD2->SolidStrokeStyle());

                 delete object;
                 delete cls;
                 delete sSurface;

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
                 pD2->pRT->SetTransform(D2D1::Matrix3x2F::Identity());
                 pD2->pRT->DrawTextW(maximumDisplayScale, maximumDisplayScale.GetLength(), textformat, D2D1::RectF(pt.x, pt.y, (FLOAT)Width, (FLOAT)Height), brush);
                 
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
                pD2->pRT->SetTransform(D2D1::Matrix3x2F::Identity());
                pD2->pRT->DrawTextW(minimumDisplayScale, minimumDisplayScale.GetLength(), textformat, D2D1::RectF(pt.x, pt.y, (FLOAT)Width, (FLOAT)Height), brush);


                pt.y += 15;
            }

            pD2->pRT->EndDraw();
        }
    }
}
