#include "stdafx.h"
#include "S100ExchangeCatalogue.h"
#include "../LatLonUtility/LatLonUtility.h"
#include "GISLibrary.h"
#include "S100Utilities.h"


S100ExchangeCatalogue::S100ExchangeCatalogue(Scaler* scaler, CatalogManager* cm, D2D1Resources* d2d1, LayerManager* parent) : SpatialObject(d2d1)
{
    m_pLayerManager = new LayerManager(scaler, cm, d2d1);
    m_pLayerManager->onIC = false;
    m_pLayerManager->m_isScreenFitEnabled = false;
    m_IsLegacy = false;
    m_pParentLayerManager = parent;
    m_ObejctType = SpatialObjectType::S100ExchangeCatalogue;
}


S100ExchangeCatalogue::~S100ExchangeCatalogue(void)
{
    if (m_pLayerManager) {
        delete m_pLayerManager;
        m_pLayerManager = nullptr;
    }
}

CString S100ExchangeCatalogue::GetFirstLayerFilePath()
{
    auto ddm = m_DataPtr->DatasetDiscoveryMetadata;

    for (int i = 0; i < ddm.size(); i++)
    {
        if (std::string::npos == ddm[i].productSpecification->ProductIdentifier.find("101") && ddm[i].productSpecification->Number != 101)
        {
            CString strFilePath = FixFileName(ddm[i].FileName);
            return strFilePath;
        }
    }
    return L"";
}



bool S100ExchangeCatalogue::Open(CString _filepath)
{
    SetFilePath(_filepath);

    m_FileType = S100_FileType::FILE_ETC;

	CT2CA pszConvertedAnsiString(_filepath);
	std::string filepath(pszConvertedAnsiString);

	m_DataPtr = make_shared<ExchangeCatalogue>();
	m_DataPtr->Open(filepath);

    auto DDM = m_DataPtr->DatasetDiscoveryMetadata;

    bool legacybound = false;
    for (const auto& item : DDM) {
        if (item.BoundingBox ) {
            legacybound = true;
            break; 
        }
    }

    if (legacybound)
    {
        for (int i = 0; i < DDM.size(); i++)
        {
            if (std::string::npos != DDM[i].productSpecification->ProductIdentifier.find("101") || DDM[i].productSpecification->Number == 101)
            {
                //add Inventory ptr
                shared_ptr<Inventory> inv = make_shared<Inventory>();

                auto bound = DDM[i].BoundingBox;
                inv->mbrBoundingBox = make_shared<MBR>(MBR(bound->WestBoundLongitude, bound->NorthBoundLatitude, bound->EastBoundLongitude, bound->SouthBoundLatitude));
                projection(*inv->mbrBoundingBox);
                inv->strFileName = DDM[i].FileName;
                inv->strFilePath = FixFileName(DDM[i].FileName);

                auto dataCoverages = DDM[i].dataCoverage;
                for (int idx = 0; idx < dataCoverages.size(); idx++)
                {
                    vector<D2D1_POINT_2F> point;

                    /*if (dataCoverages[idx].MaximumDisplayScale && dataCoverages[idx].MinimumDisplayScale)
                    {*/
                    ScaleBand sr;

                    if(dataCoverages[idx].MaximumDisplayScale != nullptr)
                        sr.OptDisplayScale = *dataCoverages[idx].MaximumDisplayScale;

                    if(dataCoverages[idx].MinimumDisplayScale != nullptr)
                        sr.MinDisplayScale = *dataCoverages[idx].MinimumDisplayScale;
                        
                        
                       
                        inv->vecScaleRange.push_back(sr);

                        //is First
                        if (idx == 0)
                        {
                            inv->totalScaleBand = sr;
                        }
                        else
                        {
                            ScaleBand sb;
                            sb.OptDisplayScale = min(inv->totalScaleBand.OptDisplayScale, sr.OptDisplayScale);
                            sb.MinDisplayScale = min(inv->totalScaleBand.MinDisplayScale, sr.MinDisplayScale);
                            inv->totalScaleBand = sb;
                        }
                    //}

                    bool reverse = true;
                    if (dataCoverages[idx].BoundingPolygon.Polygon.srsName.find("4326") != std::string::npos)
                        reverse = true;


                    for (int geoms = 0; geoms < dataCoverages[idx].BoundingPolygon.Polygon.Geom.size(); geoms++)
                    {
                        auto strPosList = LatLonUtility::Split(LatLonUtility::TrimRight(dataCoverages[idx].BoundingPolygon.Polygon.Geom[geoms]), " ");
                        int posCnt = strPosList.size();

                        if (posCnt < 4 && posCnt % 2 != 0)
                            break;

                        for (int i = 0; i < posCnt; i += 2)
                        {
                            double lon = std::stod(strPosList.at(i));
                            double lat = std::stod(strPosList.at(i + 1));

                            if (reverse)
                            {
                                projection(lat, lon);
                                point.push_back(D2D1::Point2F(lat, lon));
                            }
                            else
                            {
                                projection(lon, lat);
                                point.push_back(D2D1::Point2F(lon, lat));
                            }
                        }
                    }
                    inv->vecBoundingPolygon.push_back(point);
                }
                m_vecInventory.push_back(inv);
            }
            else
            {
                CString strFilePath = FixFileName(DDM[i].FileName);
                m_pParentLayerManager->AddLayer(strFilePath);
            }
        }
    }
    else
    {
        // if Legacy mode
        this->m_IsLegacy = true;

        for (int i = 0; i < DDM.size(); i++)
        {
            if (std::string::npos != DDM[i].productSpecification->ProductIdentifier.find("101") || DDM[i].productSpecification->Number == 101)
            {
                //add Inventory ptr
                shared_ptr<Inventory> inv = make_shared<Inventory>();

                auto bound = DDM[i].BoundingBox;

                if (!bound)
                {
                    inv->strFileName = DDM[i].FileName;
                    inv->strFilePath = FixLegacyFileName(DDM[i].FileName);
                }

                auto dataCoverages = DDM[i].dataCoverage;
                for (int idx = 0; idx < dataCoverages.size(); idx++)
                {
                    if (!bound)
                    {
                        bound = dataCoverages[idx].BoundingBox;
                        if (bound)
                        {
                            if (inv->mbrBoundingBox)
                                inv->mbrBoundingBox->ReMBR(MBR(bound->WestBoundLongitude, bound->NorthBoundLatitude, bound->EastBoundLongitude, bound->SouthBoundLatitude));
                            else
                                inv->mbrBoundingBox = make_shared<MBR>(MBR(bound->WestBoundLongitude, bound->NorthBoundLatitude, bound->EastBoundLongitude, bound->SouthBoundLatitude));
                        }

                        bound = nullptr;
                    }

                    vector<D2D1_POINT_2F> point;

                    if (dataCoverages[idx].MaximumDisplayScale && dataCoverages[idx].MinimumDisplayScale)
                    {
                        ScaleBand sr;
                        sr.OptDisplayScale = *dataCoverages[idx].MaximumDisplayScale;
                        sr.MinDisplayScale = *dataCoverages[idx].MinimumDisplayScale;
                        inv->vecScaleRange.push_back(sr);

                        //is First
                        if (idx == 0)
                        {
                            inv->totalScaleBand = sr;
                        }
                        else
                        {
                            ScaleBand sb;
                            sb.OptDisplayScale = min(inv->totalScaleBand.OptDisplayScale, sr.OptDisplayScale);
                            sb.MinDisplayScale = min(inv->totalScaleBand.MinDisplayScale, sr.MinDisplayScale);
                            inv->totalScaleBand = sb;
                        }
                    }
                    else
                    {
                        int start_index = 5;
                        int length = 3;

                        std::string extracted = DDM[i].FileName.substr(start_index, length);

                        try {
                            S100Utilities util;
                            int extracted_int = std::stoi(extracted);

                            auto sbs = util.GetLegacyScaleband(extracted_int);

                            ScaleBand sr;
                            sr.OptDisplayScale = sbs.optimumScale;
                            sr.MinDisplayScale = sbs.minimumScale;
                            inv->vecScaleRange.push_back(sr);

                            //is First
                            if (idx == 0)
                            {
                                inv->totalScaleBand = sr;
                            }
                            else
                            {
                                ScaleBand sb;
                                sb.OptDisplayScale = min(inv->totalScaleBand.OptDisplayScale, sr.OptDisplayScale);
                                sb.MinDisplayScale = min(inv->totalScaleBand.MinDisplayScale, sr.MinDisplayScale);
                                inv->totalScaleBand = sb;
                            }
                        }
                        catch (std::invalid_argument const& e) {

                        }
                    }

                    bool reverse = false;
                    if (dataCoverages[idx].BoundingPolygon.Polygon.srsName.find("4326") != std::string::npos)
                        reverse = true;


                    for (int geoms = 0; geoms < dataCoverages[idx].BoundingPolygon.Polygon.Geom.size(); geoms++)
                    {
                        auto strPosList = LatLonUtility::Split(LatLonUtility::TrimRight(dataCoverages[idx].BoundingPolygon.Polygon.Geom[geoms]), " ");
                        int posCnt = strPosList.size();

                        if (posCnt < 4 && posCnt % 2 != 0)
                            break;

                        for (int i = 0; i < posCnt; i += 2)
                        {
                            double lon = std::stod(strPosList.at(i));
                            double lat = std::stod(strPosList.at(i + 1));

                            if (reverse)
                            {
                                projection(lat, lon);
                                point.push_back(D2D1::Point2F(lat, lon));
                            }
                            else
                            {
                                projection(lon, lat);
                                point.push_back(D2D1::Point2F(lon, lat));
                            }
                        }
                    }
                    inv->vecBoundingPolygon.push_back(point);
                }
                projection(*inv->mbrBoundingBox);
                m_vecInventory.push_back(inv);
            }
            else
            {
                CString strFilePath = FixFileName(DDM[i].FileName);
                m_pParentLayerManager->AddLayer(strFilePath);
            }
        }
    }

	return true;
}

bool S100ExchangeCatalogue::OpenMetadata(CString _filepath)
{
    return true;
}

bool S100ExchangeCatalogue::CompareByLayer(const Layer* a, const Layer* b)
{
    return CompareByFileName(a->m_spatialObject->GetFilePath(), b->m_spatialObject->GetFilePath());
}

bool S100ExchangeCatalogue::CompareByFileName(const CString& a, const CString& b) 
{
    shared_ptr<Inventory> invA,invB;
    S100Utilities util;

    for (auto inventory : m_vecInventory)
    {
        if (inventory->strFilePath == a)
            invA = inventory;

        if (inventory->strFilePath == b)
            invB = inventory;
    }
    return ScaleBand::CompareByScale(invA->totalScaleBand, invB->totalScaleBand);
}

CString S100ExchangeCatalogue::FixLegacyFileName(string str)
{
    string catalogueFolderPath;
    CT2CA pszConvertedAnsiString(GetFilePath());
    string CatalogFilePath(pszConvertedAnsiString);

    size_t lastBackslashPos = CatalogFilePath.find_last_of("\\");
    if (lastBackslashPos != std::string::npos) {
        catalogueFolderPath = CatalogFilePath.substr(0, lastBackslashPos + 1);
    }

    std::string filePath = catalogueFolderPath +"DataSet\\" + str;

    CString temp(filePath.c_str());
    return temp;
}


CString S100ExchangeCatalogue::FixFileName(string str)
{
    CString catalogueFolderPath;
    CString filePath = GetFilePath(); 

    int lastBackslashPos = filePath.ReverseFind('\\');
    if (lastBackslashPos != -1) {
        catalogueFolderPath = filePath.Left(lastBackslashPos + 1);
    }

    CString result = CString(str.c_str());
    result.Replace(_T("file:/"), _T(""));
    result.Replace(_T('/'), _T('\\'));

    CString finalPath = catalogueFolderPath + result;

    return finalPath;
}


bool S100ExchangeCatalogue::containsFile(const std::vector<std::shared_ptr<InventoryItem>>& files, const CString& targetFile) {
    for (auto file : files) {
        if (file->strFilePath.Compare(targetFile) == 0) { 
            return true; 
        }
    }
    return false; 
}

void S100ExchangeCatalogue::DrawFiles(HDC& hDC, Scaler* scaler, double offset, std::vector<std::shared_ptr<InventoryItem>>& item)
{
    string catalogueFolderPath;
    MBR mbr;
    S100Utilities util;

    scaler->GetMap(&mbr);

    item = util.SelectDataCoverages(m_vecInventory, scaler, mbr);

    auto pRenderTarget = D2->pRT;
    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    
    // check Remove LayerItem 
    std::vector<CString> pathsToDelete;
    for (auto layer : m_pLayerManager->layers) {
        if (!containsFile(item, layer->GetLayerPath())) {
            pathsToDelete.push_back(layer->GetLayerPath());
        }
    }
    // Remove LayerItem 
    for (const auto& path : pathsToDelete) {
        m_pLayerManager->DeleteLayer(path);
    }

    // add Layer
    for (auto temp : item)
    {
        if (m_pLayerManager->IsContainFilePathToLayer(temp->strFilePath) == false)
            m_pLayerManager->AddLayer(temp->strFilePath);
    }

    auto ptr = this; 

    // Sort DataCoverage List
    m_pLayerManager->layers.sort([this](const Layer* a, const Layer* b){
        return CompareByFileName(a->m_spatialObject->GetFilePath(), b->m_spatialObject->GetFilePath());;
        });

}

void S100ExchangeCatalogue::DrawEx(HDC& hDC, Scaler* scaler, double offset)
{
    MBR mbr;
    S100Utilities util;
    scaler->GetMap(&mbr);
    std::vector<std::shared_ptr<InventoryItem>> item = util.SelectDataCoverages(m_vecInventory, scaler, mbr);

    auto pRenderTarget = D2->pRT;
    //pRenderTarget->SetTransform(scaler->GetMatrix());
    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

    for (int i = 0 ; i < item.size() ; i++)
    {
        auto brush = D2->pBrush;

        long x;
        long y;

        ID2D1PathGeometry* pPathGeometry = nullptr;
        D2->Factory()->CreatePathGeometry(&pPathGeometry);

        ID2D1GeometrySink* pSink = nullptr;
        pPathGeometry->Open(&pSink);

        for (int geoms = 0; geoms < item[i]->BoundingPolygon.size(); geoms++)
        {

            scaler->WorldToDevice(item[i]->BoundingPolygon[geoms].x, item[i]->BoundingPolygon[geoms].y, &x, &y);
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
        pRenderTarget->DrawGeometry(pPathGeometry, brush, 1.0, D2->SolidStrokeStyle());
        pRenderTarget->EndDraw();

        pSink->Release();
        pPathGeometry->Release();
    }
}


void S100ExchangeCatalogue::Draw(HDC& hDC, Scaler* scaler, double offset)
{
    std::vector<std::shared_ptr<InventoryItem>> items;

    DrawFiles(hDC, scaler, offset, items);

    m_pLayerManager->Draw(hDC, offset);

    double Width = scaler->GetScreenWidth();
    double Height = scaler->GetScreenWidth();

    D2->pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    D2->pDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

    auto pRenderTarget = D2->pRT;


    if (this->m_IsLegacy)
    {
        //pRenderTarget->BeginDraw();
        //for (int i = 0; i < m_DataPtr->DatasetDiscoveryMetadata.size(); i++)
        //{
        //    
        //    auto textformat = D2->pDWriteTextFormat;
        //    auto brush = D2->pBrush;
        //    auto ddm = m_DataPtr->DatasetDiscoveryMetadata[i];

        //    //auto object = new GM::Surface();
        //    //auto curve = new GM::Curve();

        //    //auto bb = ddm.BoundingBox;

        //    //double lon = bb->WestBoundLongitude;
        //    //double lat = bb->NorthBoundLatitude;
        //    //curve->Add(lon, lat);

        //    //lon = bb->EastBoundLongitude;
        //    //lat = bb->NorthBoundLatitude;
        //    //curve->Add(lon, lat);

        //    //lon = bb->EastBoundLongitude;
        //    //lat = bb->SouthBoundLatitude;
        //    //curve->Add(lon, lat);

        //    //lon = bb->WestBoundLongitude;
        //    //lat = bb->SouthBoundLatitude;
        //    //curve->Add(lon, lat);

        //    //object->SetExteriorRing(curve);

        //    //S10XGML* cls = new S10XGML(D2);
        //    //auto sSurface = cls->SurfaceToSSurface(object);
        //    //sSurface->CreateD2Geometry(D2->Factory());
        //    //brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
        //    //pRenderTarget->SetTransform(scaler->GetMatrix());
        //    //pRenderTarget->DrawGeometry(sSurface->GetD2Geometry(), brush, 1.0, D2->SolidStrokeStyle());

        //    //delete object;
        //    //delete cls;
        //    //delete curve;
        //    //delete sSurface;

        //    //D2D1_POINT_2F pt;
        //    //pt.x = bb->WestBoundLongitude;
        //    //pt.y = bb->NorthBoundLatitude;

        //    //projection(pt.x, pt.y);

        //    //scaler->WorldToDevice(pt);

        //    //CString cstrFileName = CString(ddm.FileName.c_str());
        //    //CString fileName = L"FileName : " + cstrFileName;
        //    //brush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
        //    //pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

        //    //pRenderTarget->DrawTextW(fileName, fileName.GetLength(), textformat, D2D1::RectF(pt.x, pt.y, pt.x + 5, pt.y + 5), brush);

        //    //Datacorverage
        //    for (int j = 0; j < ddm.dataCoverage.size(); j++)
        //    {
        //        D2->pRT->SetTransform(scaler->GetMatrix());

        //        auto dc = ddm.dataCoverage[j];
        //        auto object = new GM::Surface();

        //        bool reverse = false;

        //        if (dc.BoundingPolygon.Polygon.srsName.find("4326") != std::string::npos)
        //            reverse = true;

        //        for (int geoms = 0; geoms < dc.BoundingPolygon.Polygon.Geom.size(); geoms++)
        //        {
        //            auto curve = new GM::Curve();

        //            auto strPosList = LatLonUtility::Split(LatLonUtility::TrimRight(dc.BoundingPolygon.Polygon.Geom[geoms]), " ");
        //            int posCnt = strPosList.size();

        //            if (posCnt < 4 && posCnt % 2 != 0)
        //                break;

        //            for (int i = 0; i < posCnt; i += 2)
        //            {
        //                double lon = std::stod(strPosList.at(i));
        //                double lat = std::stod(strPosList.at(i + 1));

        //                if (reverse)
        //                    curve->Add(lat, lon);
        //                else
        //                    curve->Add(lon, lat);
        //            }
        //            object->SetExteriorRing(curve);

        //            delete curve;
        //        }

        //        S10XGML* cls = new S10XGML(D2);
        //        auto sSurface = cls->SurfaceToSSurface(object);
        //        sSurface->CreateD2Geometry(D2->Factory());
        //        //brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
        //        //brush->SetOpacity(0.3);
        //        //pRenderTarget->FillGeometry(sSurface->GetD2Geometry(), brush);

        //        brush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
        //        pRenderTarget->DrawGeometry(sSurface->GetD2Geometry(), brush, 1.0, D2->SolidStrokeStyle());

        //        delete object;
        //        delete cls;
        //        delete sSurface;

        //        //pt.y += 15;
        //        //////maximumDisplayScale 
        //        //CString maximumDisplayScale;
        //        //if (dc.MaximumDisplayScale != nullptr)
        //        //{
        //        //    CString str(std::to_string(*dc.MaximumDisplayScale).c_str());
        //        //    maximumDisplayScale = L"MaximumDisplayScale : " + str;
        //        //}
        //        //else
        //        //    maximumDisplayScale = L"MaximumDisplayScale : - ";
        //        //brush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
        //        //brush->SetOpacity(1);
        //        //pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        //        //pRenderTarget->DrawTextW(maximumDisplayScale, maximumDisplayScale.GetLength(), textformat, D2D1::RectF(pt.x, pt.y, pt.x + 5, pt.y + 5), brush);

        //        //pt.y += 15;
        //        //////minimumDisplayScale 
        //        //CString minimumDisplayScale;
        //        //if (dc.MinimumDisplayScale != nullptr)
        //        //{
        //        //    CString str(std::to_string(*dc.MinimumDisplayScale).c_str());
        //        //    minimumDisplayScale = L"MinimumDisplayScale : " + str;
        //        //}
        //        //else
        //        //    minimumDisplayScale = L"MinimumDisplayScale : - ";
        //        //brush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
        //        //pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        //        //pRenderTarget->DrawTextW(minimumDisplayScale, minimumDisplayScale.GetLength(), textformat, D2D1::RectF(pt.x, pt.y, pt.x + 5, pt.y + 5), brush);

        //        //pt.y += 15;
        //    }

        //    
        //}
        //pRenderTarget->EndDraw();
    }
    else
    {
        pRenderTarget->BeginDraw();

        // 현재 스케일에 맞는 아이템만 처리
        for (const auto& item : items)
        {
            // items에 있는 파일명과 일치하는 DatasetDiscoveryMetadata 찾기
            for (int i = 0; i < m_DataPtr->DatasetDiscoveryMetadata.size(); i++)
            {
                auto& ddm = m_DataPtr->DatasetDiscoveryMetadata[i];
                CString fixedFileName = FixFileName(ddm.FileName);
                
                if (item->strFilePath != fixedFileName)
                    continue;

                if (std::string::npos != ddm.productSpecification->ProductIdentifier.find("101"))
                {
                    auto textformat = D2->pDWriteTextFormat;
                    auto brush = D2->pBrush;

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

                    S10XGML* cls = new S10XGML(D2);
                    auto sSurface = cls->SurfaceToSSurface(object);
                    sSurface->CreateD2Geometry(D2->Factory());
                    brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
                    pRenderTarget->SetTransform(scaler->GetMatrix());
                    pRenderTarget->DrawGeometry(sSurface->GetD2Geometry(), brush, 1.0, D2->SolidStrokeStyle());

                    delete object;
                    delete cls;
                    delete curve;
                    delete sSurface;

                    D2D1_POINT_2F ptTopLeft;
                    D2D1_POINT_2F ptBottomRight;
                    ptTopLeft.x = bb->WestBoundLongitude;
                    ptTopLeft.y = bb->NorthBoundLatitude;
                    ptBottomRight.x = bb->EastBoundLongitude;
                    ptBottomRight.y = bb->SouthBoundLatitude;

                    projection(ptTopLeft.x, ptTopLeft.y);
                    projection(ptBottomRight.x, ptBottomRight.y);

                    scaler->WorldToDevice(ptTopLeft);
                    scaler->WorldToDevice(ptBottomRight);

                    // 스크린 좌표에서의 사각형 크기 계산
                    float rectWidth = abs(ptBottomRight.x - ptTopLeft.x);
                    float rectHeight = abs(ptBottomRight.y - ptTopLeft.y);

                    CString cstrFileName = CString(ddm.FileName.c_str());
                    CString fileName = L"FileName : " + cstrFileName;

                    // 사각형 크기 기반으로 폰트 크기 계산 (한 줄로 들어오도록)
                    float fontSize = 12.0f;
                    IDWriteTextFormat* dynamicTextFormat = nullptr;
                    IDWriteTextLayout* textLayout = nullptr;
                    bool canDrawText = false;
                    
                    // 최대 폰트 크기를 사각형 높이의 30%로 제한
                    float maxFontSize = min(50.0f, rectHeight * 0.3f);
                    float minFontSize = 10.0f;
                    
                    // 폰트 크기를 사각형에 맞춰 조정 (한 줄로 표시되도록)
                    for (fontSize = maxFontSize; fontSize >= minFontSize; fontSize -= 1.0f)
                    {
                        if (dynamicTextFormat) dynamicTextFormat->Release();
                        
                        D2->WriteFactory()->CreateTextFormat(
                            L"Arial",
                            nullptr,
                            DWRITE_FONT_WEIGHT_NORMAL,
                            DWRITE_FONT_STYLE_NORMAL,
                            DWRITE_FONT_STRETCH_NORMAL,
                            fontSize,
                            L"ko-kr",
                            &dynamicTextFormat
                        );

                        if (textLayout) textLayout->Release();
                        D2->WriteFactory()->CreateTextLayout(
                            fileName,
                            fileName.GetLength(),
                            dynamicTextFormat,
                            rectWidth * 0.95f,  // 가로 여백 5%
                            rectHeight,
                            &textLayout
                        );

                        DWRITE_TEXT_METRICS textMetrics;
                        textLayout->GetMetrics(&textMetrics);

                        // 한 줄로 들어오고 사각형 안에 들어오며 폰트 크기가 50 이하인지 확인
                        if (textMetrics.lineCount <= 1 && 
                            textMetrics.width <= rectWidth * 0.95f && 
                            textMetrics.height <= rectHeight * 0.3f &&
                            fontSize <= 50.0f)
                        {
                            canDrawText = true;
                            break;
                        }
                    }

                    // 텍스트 그리기 (스크린 좌표 기반, 한 줄에 들어올 때만)
                    if (canDrawText)
                    {
                        brush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
                        brush->SetOpacity(1.0f);
                        pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

                        // 좌상단 좌표 계산
                        float left = min(ptTopLeft.x, ptBottomRight.x);
                        float top = min(ptTopLeft.y, ptBottomRight.y);
                        float right = max(ptTopLeft.x, ptBottomRight.x);
                        float bottom = max(ptTopLeft.y, ptBottomRight.y);

                        // 텍스트를 좌상단에 표시하기 위한 영역 설정
                        D2D1_RECT_F textRect = D2D1::RectF(
                            left,
                            top,
                            right,
                            top + fontSize * 1.2f  // 폰트 크기에 맞춰 높이 설정
                        );

                        if (dynamicTextFormat)
                        {
                            dynamicTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);  // 왼쪽 정렬
                            dynamicTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);  // 위쪽 정렬
                            pRenderTarget->DrawTextW(fileName, fileName.GetLength(), dynamicTextFormat, textRect, brush);
                            dynamicTextFormat->Release();
                        }

                        if (textLayout) textLayout->Release();

                        //Datacorverage
                        // 텍스트 간 간격 설정 (파일명 폰트 크기의 20%)
                        float textSpacing = fontSize * 0.2f;
                        float currentY = textRect.bottom + textSpacing;
                    
                    for (int j = 0; j < ddm.dataCoverage.size(); j++)
                    {
                        D2->pRT->SetTransform(scaler->GetMatrix());

                        auto dc = ddm.dataCoverage[j];
                        auto object = new GM::Surface();

                        bool reverse = false;

                        if (dc.BoundingPolygon.Polygon.srsName.find("4326") != std::string::npos)
                            reverse = true;

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

                                if (reverse)
                                    curve->Add(lat, lon);
                                else
                                    curve->Add(lon, lat);
                            }
                            object->SetExteriorRing(curve);

                            delete curve;
                        }

                        S10XGML* cls = new S10XGML(D2);
                        auto sSurface = cls->SurfaceToSSurface(object);
                        sSurface->CreateD2Geometry(D2->Factory());

                        delete object;
                        delete cls;
                        delete sSurface;

                        CString maximumDisplayScale;
                        if (dc.MaximumDisplayScale != nullptr)
                        {
                            CString str(std::to_string(*dc.MaximumDisplayScale).c_str());
                            maximumDisplayScale = L"Max Scale: " + str;
                        }
                        else
                            maximumDisplayScale = L"Max Scale: -";

                        CString minimumDisplayScale;
                        if (dc.MinimumDisplayScale != nullptr)
                        {
                            CString str(std::to_string(*dc.MinimumDisplayScale).c_str());
                            minimumDisplayScale = L"Min Scale: " + str;
                        }
                        else
                            minimumDisplayScale = L"Min Scale: -";

                        // 스케일 정보를 사각형 크기에 맞게 동적으로 계산 (파일명의 60% 크기)
                        float scaleFontSize = fontSize * 0.6f;
                        float maxScaleFontSize = 30.0f;  // 스케일 정보는 최대 30
                        float minScaleFontSize = 10.0f;
                        
                        if (scaleFontSize > maxScaleFontSize) scaleFontSize = maxScaleFontSize;
                        if (scaleFontSize < minScaleFontSize) scaleFontSize = minScaleFontSize;
                        
                        IDWriteTextFormat* scaleTextFormat = nullptr;
                        IDWriteTextLayout* scaleLayout = nullptr;
                        bool canDrawScale = false;

                        // 스케일 정보 폰트 크기 조정 (한 줄로 들어오도록)
                        CString longerText = maximumDisplayScale.GetLength() > minimumDisplayScale.GetLength() ? maximumDisplayScale : minimumDisplayScale;
                        
                        for (float testSize = scaleFontSize; testSize >= minScaleFontSize; testSize -= 1.0f)
                        {
                            if (scaleTextFormat) scaleTextFormat->Release();
                            
                            D2->WriteFactory()->CreateTextFormat(
                                L"Arial",
                                nullptr,
                                DWRITE_FONT_WEIGHT_NORMAL,
                                DWRITE_FONT_STYLE_NORMAL,
                                DWRITE_FONT_STRETCH_NORMAL,
                                testSize,
                                L"ko-kr",
                                &scaleTextFormat
                            );

                            if (scaleLayout) scaleLayout->Release();
                            D2->WriteFactory()->CreateTextLayout(
                                longerText,
                                longerText.GetLength(),
                                scaleTextFormat,
                                rectWidth * 0.95f,
                                rectHeight,
                                &scaleLayout
                            );

                            DWRITE_TEXT_METRICS scaleMetrics;
                            scaleLayout->GetMetrics(&scaleMetrics);

                            if (scaleMetrics.lineCount <= 1 && 
                                scaleMetrics.width <= rectWidth * 0.95f &&
                                testSize <= maxScaleFontSize)
                            {
                                scaleFontSize = testSize;
                                canDrawScale = true;
                                break;
                            }
                        }

                        if (scaleLayout) scaleLayout->Release();

                        if (canDrawScale && scaleTextFormat)
                        {
                            scaleTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
                            scaleTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

                            brush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
                            brush->SetOpacity(1);
                            pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

                            D2D1_RECT_F maxScaleRect = D2D1::RectF(
                                left,
                                currentY,
                                right,
                                currentY + scaleFontSize * 1.2f
                            );
                            
                            pRenderTarget->DrawTextW(maximumDisplayScale, maximumDisplayScale.GetLength(), scaleTextFormat, maxScaleRect, brush);
                            currentY += scaleFontSize * 1.2f + textSpacing;

                            D2D1_RECT_F minScaleRect = D2D1::RectF(
                                left,
                                currentY,
                                right,
                                currentY + scaleFontSize * 1.2f
                            );
                            
                            pRenderTarget->DrawTextW(minimumDisplayScale, minimumDisplayScale.GetLength(), scaleTextFormat, minScaleRect, brush);
                            currentY += scaleFontSize * 1.2f + textSpacing;  // 다음 dataCoverage를 위해 Y 위치 증가
                        }
                        
                        if (scaleTextFormat) scaleTextFormat->Release();
                    }
                    } // canDrawText
                    else
                    {
                        // 텍스트를 그릴 수 없으면 리소스 정리
                        if (dynamicTextFormat) dynamicTextFormat->Release();
                        if (textLayout) textLayout->Release();
                    }
                }
                break; // 일치하는 항목 찾았으므로 내부 루프 종료
            }
        }
        pRenderTarget->EndDraw();
    }

}

