#include "stdafx.h"
#include "S100ExchangeCatalogue.h"
#include "../LatLonUtility/LatLonUtility.h"
#include "GISLibrary.h"


S100ExchangeCatalogue::S100ExchangeCatalogue(void)
{

}


S100ExchangeCatalogue::~S100ExchangeCatalogue(void)
{

}


bool S100ExchangeCatalogue::Open(CString _filepath)
{
    SetFilePath(_filepath);

    m_FileType = S100_FileType::FILE_ETC;


	CT2CA pszConvertedAnsiString(_filepath);
	std::string filepath(pszConvertedAnsiString);

	m_DataPtr = make_shared<ExchangeCatalogue>();
	m_DataPtr->Open(filepath);

	return true;
}


void S100ExchangeCatalogue::Draw(HDC& hDC, Scaler* scaler, double offset)
{
    double Width = gisLib->GetScaler()->GetScreenWidth();
    double Height = gisLib->GetScaler()->GetScreenWidth();

    gisLib->D2.pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    gisLib->D2.pDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

    auto pRenderTarget = gisLib->D2.pRT;
         
    for (int i = 0; i < m_DataPtr->DatasetDiscoveryMetadata.size(); i++)
    {
        pRenderTarget->BeginDraw();
        auto textformat = gisLib->D2.pDWriteTextFormat;
        auto brush = gisLib->D2.pBrush;
        auto ddm = m_DataPtr->DatasetDiscoveryMetadata[i];

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
        delete curve;
        delete sSurface;

        D2D1_POINT_2F pt;
        pt.x = bb->WestBoundLongitude;
        pt.y = bb->NorthBoundLatitude;

        projection(pt.x, pt.y);

        gisLib->WorldToDevice(pt);

        CString cstrFileName = CString(ddm.FileName.c_str());
        CString fileName = L"FileName : " + cstrFileName;
        brush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
        pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

        pRenderTarget->DrawTextW(fileName, fileName.GetLength(), textformat, D2D1::RectF(pt.x, pt.y, pt.x + 5, pt.y + 5), brush);

        //Datacorverage
        for (int j = 0; j < ddm.dataCoverage.size(); j++)
        {
            gisLib->D2.pRT->SetTransform(scaler->GetMatrix());

            auto dc = ddm.dataCoverage[j];
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

                delete curve;
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

