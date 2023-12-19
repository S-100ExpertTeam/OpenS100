#include "stdafx.h"
#include "S100ExchangeCatalogue.h"
#include "../LatLonUtility/LatLonUtility.h"
#include "GISLibrary.h"
#include "S100Utilities.h"


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

    //
    auto DDM = m_DataPtr->DatasetDiscoveryMetadata;

    for (int i = 0; i < DDM.size(); i++)
    {
        //add Inventory ptr
        shared_ptr<Inventory> inv = make_shared<Inventory>();
        auto bound = DDM[i].BoundingBox;
        inv->mbrBoundingBox = MBR(bound->WestBoundLongitude, bound->NorthBoundLatitude, bound->EastBoundLongitude, bound->SouthBoundLatitude);
        projection(inv->mbrBoundingBox);
        inv->strFileName = DDM[i].FileName;

        auto dataCoverages = DDM[i].dataCoverage;
        for (int idx = 0; idx < dataCoverages.size(); idx++)
        {
            vector<D2D1_POINT_2F> point;
            ScaleBand sr;
            sr.MaxDisplayScale = *dataCoverages[idx].MaximumDisplayScale;
            sr.MinDisplayScale = *dataCoverages[idx].MinimumDisplayScale;

            inv->vecScaleRange.push_back(sr);


            //auto object = new GM::Surface();
            for (int geoms = 0; geoms < dataCoverages[idx].BoundingPolygon.Polygon.Geom.size(); geoms++)
            {
                //auto curve = new GM::Curve();

                auto strPosList = LatLonUtility::Split(LatLonUtility::TrimRight(dataCoverages[idx].BoundingPolygon.Polygon.Geom[geoms]), " ");
                int posCnt = strPosList.size();

                if (posCnt < 4 && posCnt % 2 != 0)
                    break;

                for (int i = 0; i < posCnt; i += 2)
                {
                    double lon = std::stod(strPosList.at(i));
                    double lat = std::stod(strPosList.at(i + 1));
                    projection(lon, lat);
                    
                    point.push_back(D2D1::Point2F(lon, lat));

                }
            }
            inv->vecBoundingPolygon.push_back(point);
        }
        m_vecInventory.push_back(inv);
    }

	return true;
}

void S100ExchangeCatalogue::DrawFiles(HDC& hDC, Scaler* scaler, double offset)
{
    MBR mbr;
    S100Utilities util;
    gisLib->GetMap(&mbr);
    std::vector<std::shared_ptr<InventoryItem>> item = util.SelectDataCoverages(m_vecInventory, scaler->GetCurrentScale(), mbr);

    auto pRenderTarget = gisLib->D2.pRT;
    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

    //Revers vector item 
    for (int i = item.size() -1; i >= 0 ; i --)
    {
        std::string file = item[i]->strFileName;
        std::string toRemove = "file:";
        std::string result;

        size_t pos = file.find(toRemove);
        if (pos != std::string::npos) {
            result = file.substr(pos + toRemove.length());
        }

        CString CatalogFilePath = GetFilePath();


        //gisLib->AddLayer();

    }
}

void S100ExchangeCatalogue::DrawEx(HDC& hDC, Scaler* scaler, double offset)
{
    MBR mbr;
    S100Utilities util;
    gisLib->GetMap(&mbr);
    std::vector<std::shared_ptr<InventoryItem>> item = util.SelectDataCoverages(m_vecInventory, scaler->GetCurrentScale(), mbr);

    auto pRenderTarget = gisLib->D2.pRT;
    //pRenderTarget->SetTransform(scaler->GetMatrix());
    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

    for (int i = 0 ; i < item.size() ; i++)
    {
        auto brush = gisLib->D2.pBrush;

        long x;
        long y;

        ID2D1PathGeometry* pPathGeometry = nullptr;
        gisLib->D2.Factory()->CreatePathGeometry(&pPathGeometry);

        ID2D1GeometrySink* pSink = nullptr;
        pPathGeometry->Open(&pSink);


        

        for (int geoms = 0; geoms < item[i]->BoundingPolygon.size(); geoms++)
        {

            gisLib->GetScaler()->WorldToDevice(item[i]->BoundingPolygon[geoms].x, item[i]->BoundingPolygon[geoms].y, &x, &y);
            if( geoms ==0)
                pSink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_FILLED);
            else
                pSink->AddLine(D2D1::Point2F(x, y));
        }

        pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
        pSink->Close();

        pRenderTarget->BeginDraw();
        brush->SetColor(D2D1::ColorF(util.GetColorNum(i)));
        brush->SetOpacity(0.3);
        pRenderTarget->FillGeometry(pPathGeometry, brush);

        brush->SetColor(D2D1::ColorF(util.GetColorNum(i)));
        pRenderTarget->DrawGeometry(pPathGeometry, brush, 1.0, gisLib->D2.SolidStrokeStyle());
        pRenderTarget->EndDraw();

        pSink->Release();
        pPathGeometry->Release();


    }

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
            //brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
            //brush->SetOpacity(0.3);
            //pRenderTarget->FillGeometry(sSurface->GetD2Geometry(), brush);

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


    //DrawFiles(hDC, scaler, offset);
    DrawEx(hDC, scaler, offset);
}

