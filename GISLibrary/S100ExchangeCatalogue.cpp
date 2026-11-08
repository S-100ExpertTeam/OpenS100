#include "stdafx.h"
#include "PathUtil.h"
#include "TextCompare.h"
#include "../GeoMetryLibrary/TextCodec.h"
#include <algorithm>
#include <filesystem>
#include "D2DView.h"
#include "S100ExchangeCatalogue.h"
#include "D2DGeometryCache.h"
#include "GISLibrary.h"
#include "Inventory.h"
#include "S100Utilities.h"

#include "..\\LatLonUtility\\LatLonUtility.h"
#include "..\\LatLonUtility\\cpp_util.h"

S100ExchangeCatalogue::S100ExchangeCatalogue(Scaler* scaler, CatalogManager* cm, D2D1Resources* d2d1, LayerManager* parent) : SpatialObject()
{
    D2 = d2d1;
    m_pLayerManager = new LayerManager(scaler, cm, d2d1);
    m_pLayerManager->onIC = false;
    m_pLayerManager->m_isScreenFitEnabled = false;
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

std::wstring S100ExchangeCatalogue::GetFirstLayerFilePath()
{
    auto ddm = m_DataPtr->DatasetDiscoveryMetadata;

    for (int i = 0; i < ddm.size(); i++)
    {
        if (std::string::npos == ddm[i].productSpecification->ProductIdentifier.find("101") && ddm[i].productSpecification->Number != 101)
        {
            const std::wstring strFilePath = FixFileName(ddm[i].FileName);
            return strFilePath;
        }
    }
    return L"";
}



bool S100ExchangeCatalogue::Open(const std::wstring& _filepath)
{
    SetFilePath(_filepath);

    m_FileType = S100_FileType::FILE_ETC;

	// Narrow path in the native encoding (ACP on Windows, UTF-8 elsewhere) for the narrow XML loader.
	const std::string filepath = std::filesystem::path(_filepath).string();

	m_DataPtr = std::make_shared<ExchangeCatalogue>();
	m_DataPtr->Open(filepath);

    auto DDM = m_DataPtr->DatasetDiscoveryMetadata;

    for (int i = 0; i < DDM.size(); i++)
    {
        if (std::string::npos != DDM[i].productSpecification->ProductIdentifier.find("101") || DDM[i].productSpecification->Number == 101)
        {
            //add Inventory ptr
            std::shared_ptr<Inventory> inv = std::make_shared<Inventory>();

            auto bound = DDM[i].BoundingBox;
            inv->mbrBoundingBox = std::make_shared<MBR>(MBR(bound->WestBoundLongitude, bound->NorthBoundLatitude, bound->EastBoundLongitude, bound->SouthBoundLatitude));
            projection(*inv->mbrBoundingBox);
            inv->strFileName = DDM[i].FileName;
            inv->strFilePath = FixFileName(DDM[i].FileName);

            auto dataCoverages = DDM[i].dataCoverage;
            for (int idx = 0; idx < dataCoverages.size(); idx++)
            {
                std::vector<D2D1_POINT_2F> point;

                ScaleBand sr;

                if (dataCoverages[idx].OptimumDisplayScale != nullptr) {
                    sr.SetOptimumDisplayScale(*dataCoverages[idx].OptimumDisplayScale);
                }

                if (dataCoverages[idx].MinimumDisplayScale != nullptr) {
                    sr.SetMinimumDisplayScale(*dataCoverages[idx].MinimumDisplayScale);
                }
                        
                inv->vecScaleRange.push_back(sr);

                //is First
                if (idx == 0)
                {
                    inv->totalScaleBand = sr;
                }
                else
                {
                    ScaleBand sb;
                    sb.SetOptimumDisplayScale(min(inv->totalScaleBand.GetOptimumDisplayScale(), sr.GetOptimumDisplayScale()));
                    sb.SetMinimumDisplayScale(min(inv->totalScaleBand.GetMinimumDisplayScale(), sr.GetMinimumDisplayScale()));
                    inv->totalScaleBand = sb;
                }

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
                        double lon = cpp_util::stod(strPosList.at(i));
                        double lat = cpp_util::stod(strPosList.at(i + 1));

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
            const std::wstring strFilePath = FixFileName(DDM[i].FileName);
            m_pParentLayerManager->AddLayer(strFilePath);
        }
    }

	return true;
}

bool S100ExchangeCatalogue::OpenMetadata(const std::wstring& _filepath)
{
    return true;
}

bool S100ExchangeCatalogue::CompareByLayer(const Layer* a, const Layer* b)
{
    return CompareByFileName(a->m_spatialObject->GetFilePath(), b->m_spatialObject->GetFilePath());
}

bool S100ExchangeCatalogue::CompareByFileName(const std::wstring& a, const std::wstring& b) 
{
    std::shared_ptr<Inventory> invA,invB;
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

std::wstring S100ExchangeCatalogue::FixLegacyFileName(const std::string& str)
{
    // "<catalogue dir>\\DataSet\\<name>" (UTF-8 name from the XML).
    return s100::path::Directory(GetFilePath()) + L"DataSet\\" + s100::text::ToWide(str);
}

std::wstring S100ExchangeCatalogue::FixFileName(const std::string& str)
{
    // "file:/DataSet/x.000" -> "<catalogue dir>\\DataSet\\x.000".
    std::wstring result = s100::text::ToWide(str);
    for (auto pos = result.find(L"file:/"); pos != std::wstring::npos; pos = result.find(L"file:/", pos))
    {
        result.erase(pos, 6);
    }
    std::replace(result.begin(), result.end(), L'/', L'\\');
    return s100::path::Directory(GetFilePath()) + result;
}

bool S100ExchangeCatalogue::containsFile(const std::vector<std::shared_ptr<InventoryItem>>& files, const std::wstring& targetFile) {
    for (auto file : files) {
        if (file->strFilePath == targetFile) { 
            return true; 
        }
    }
    return false; 
}

void S100ExchangeCatalogue::DrawFiles(HDC& hDC, Scaler* scaler, double offset, std::vector<std::shared_ptr<InventoryItem>>& item)
{
    std::string catalogueFolderPath;
    MBR mbr;
    S100Utilities util;

    scaler->GetMap(&mbr);

    item = util.SelectDataCoverages(m_vecInventory, scaler, mbr);

    auto pRenderTarget = D2->pRT;
    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    
    // check Remove LayerItem 
    std::vector<std::wstring> pathsToDelete;
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

    pRenderTarget->BeginDraw();

    // 현재 스케일에 맞는 아이템만 처리
    for (const auto& item : items)
    {
        // items에 있는 파일명과 일치하는 DatasetDiscoveryMetadata 찾기
        for (int i = 0; i < m_DataPtr->DatasetDiscoveryMetadata.size(); i++)
        {
            auto& ddm = m_DataPtr->DatasetDiscoveryMetadata[i];
            const std::wstring fixedFileName = FixFileName(ddm.FileName);
                
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

                S10XGML* cls = new S10XGML();
                auto sSurface = cls->SurfaceToSSurface(object);
                brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
                pRenderTarget->SetTransform(GISLibrary::D2DView::Matrix(*scaler));
                pRenderTarget->DrawGeometry(GISLibrary::D2DGeometry::Get(sSurface, D2->Factory()), brush, 1.0, D2->SolidStrokeStyle());

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

                scaler->WorldToDevice_F(ptTopLeft.x, ptTopLeft.y, &ptTopLeft.x, &ptTopLeft.y);
                scaler->WorldToDevice_F(ptBottomRight.x, ptBottomRight.y, &ptBottomRight.x, &ptBottomRight.y);

                // 스크린 좌표에서의 사각형 크기 계산
                float rectWidth = abs(ptBottomRight.x - ptTopLeft.x);
                float rectHeight = abs(ptBottomRight.y - ptTopLeft.y);

                const std::wstring fileName = L"FileName : " + s100::text::ToWide(ddm.FileName);

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
                        fileName.c_str(),
                        static_cast<UINT32>(fileName.size()),
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
                        pRenderTarget->DrawTextW(fileName.c_str(), static_cast<UINT32>(fileName.size()), dynamicTextFormat, textRect, brush);
                        dynamicTextFormat->Release();
                    }

                    if (textLayout) textLayout->Release();

                    //Datacorverage
                    // 텍스트 간 간격 설정 (파일명 폰트 크기의 20%)
                    float textSpacing = fontSize * 0.2f;
                    float currentY = textRect.bottom + textSpacing;
                    
                for (int j = 0; j < ddm.dataCoverage.size(); j++)
                {
                    D2->pRT->SetTransform(GISLibrary::D2DView::Matrix(*scaler));

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
                            double lon = cpp_util::stod(strPosList.at(i));
                            double lat = cpp_util::stod(strPosList.at(i + 1));

                            if (reverse)
                                curve->Add(lat, lon);
                            else
                                curve->Add(lon, lat);
                        }
                        object->SetExteriorRing(curve);

                        delete curve;
                    }

                    S10XGML* cls = new S10XGML();
                    auto sSurface = cls->SurfaceToSSurface(object);

                    delete object;
                    delete cls;
                    delete sSurface;

                    std::wstring optimumDisplayScale;
                    if (dc.OptimumDisplayScale != nullptr) {
                                                optimumDisplayScale = L"Opt Scale: " + std::to_wstring(*dc.OptimumDisplayScale);
                    }
                    else {
                        optimumDisplayScale = L"Opt Scale: -";
                    }
                    
                    std::wstring minimumDisplayScale;
                    if (dc.MinimumDisplayScale != nullptr) {
                                                minimumDisplayScale = L"Min Scale: " + std::to_wstring(*dc.MinimumDisplayScale);
                    }
                    else {
                        minimumDisplayScale = L"Min Scale: -";
                    }

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
                    const std::wstring& longerText = optimumDisplayScale.size() > minimumDisplayScale.size() ? optimumDisplayScale : minimumDisplayScale;
                        
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
                            longerText.c_str(),
                            static_cast<UINT32>(longerText.size()),
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

                    if (scaleLayout) {
                        scaleLayout->Release();
                    }

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
                            
                        pRenderTarget->DrawTextW(optimumDisplayScale.c_str(), static_cast<UINT32>(optimumDisplayScale.size()), scaleTextFormat, maxScaleRect, brush);
                        currentY += scaleFontSize * 1.2f + textSpacing;

                        D2D1_RECT_F minScaleRect = D2D1::RectF(
                            left,
                            currentY,
                            right,
                            currentY + scaleFontSize * 1.2f
                        );
                            
                        pRenderTarget->DrawTextW(minimumDisplayScale.c_str(), static_cast<UINT32>(minimumDisplayScale.size()), scaleTextFormat, minScaleRect, brush);
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

