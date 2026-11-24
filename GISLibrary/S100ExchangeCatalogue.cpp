#include "stdafx.h"
#include "PathUtil.h"
#include "TextCompare.h"
#include "../GeoMetryLibrary/TextCodec.h"
#include <algorithm>
#include <filesystem>
#include "S100ExchangeCatalogue.h"
#include "DatasetOpening.h"
#include "IRenderer.h"
#include "ViewFrame.h"
#include "GISLibrary.h"
#include "Inventory.h"
#include "S100Utilities.h"
#include "SdbReaders.h"
#include "ViewSelection.h"

#include "../LatLonUtility/LatLonUtility.h"
#include "../LatLonUtility/cpp_util.h"

S100ExchangeCatalogue::S100ExchangeCatalogue(Scaler* scaler, CatalogManager* cm, LayerManager* parent) : SpatialObject()
{
    m_pLayerManager = new LayerManager(scaler, cm);
    m_pLayerManager->onIC = false;
    m_pLayerManager->m_isScreenFitEnabled = false;
    m_pParentLayerManager = parent;
    m_ObejctType = SpatialObjectType::S100ExchangeCatalogue;
}


S100ExchangeCatalogue::~S100ExchangeCatalogue(void)
{
    m_layers.reset();   // its layers live in m_pLayerManager
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
    std::vector<s100::sdb::SelectableCoverage> selectable;
    m_coverageOwners.clear();

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
                std::vector<GISLibrary::PointF> point;

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
                    sb.SetOptimumDisplayScale((std::min)(inv->totalScaleBand.GetOptimumDisplayScale(), sr.GetOptimumDisplayScale()));
                    sb.SetMinimumDisplayScale((std::min)(inv->totalScaleBand.GetMinimumDisplayScale(), sr.GetMinimumDisplayScale()));
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

                    for (int _i = 0; _i < posCnt; _i += 2)
                    {
                        double lon = cpp_util::stod(strPosList.at(_i));
                        double lat = cpp_util::stod(strPosList.at(_i + 1));

                        if (reverse)
                        {
                            projection(lat, lon);
                            point.push_back(GISLibrary::PointF{ static_cast<float>(lat), static_cast<float>(lon) });
                        }
                        else
                        {
                            projection(lon, lat);
                            point.push_back(GISLibrary::PointF{ static_cast<float>(lon), static_cast<float>(lat) });
                        }
                    }
                }
                inv->vecBoundingPolygon.push_back(point);
            }
            const auto coverages = GISLibrary::exchangeCatalogueCoverages(dataCoverages);
            for (std::size_t j = 0; j < coverages.size(); ++j)
            {
                auto coverage = s100::sdb::makeSelectableCoverage(static_cast<s100::sdb::RecordId>(m_vecInventory.size()),
                    coverages[j].minimumDisplayScale, coverages[j].optimumDisplayScale, coverages[j].rings);
                if (coverage)
                {
                    selectable.push_back(std::move(*coverage));
                    m_coverageOwners.emplace_back(m_vecInventory.size(), j);
                }
            }
            m_vecInventory.push_back(inv);
        }
        else
        {
            const std::wstring strFilePath = FixFileName(DDM[i].FileName);
            m_pParentLayerManager->AddLayer(strFilePath);
        }
    }
    m_selector = s100::sdb::DatasetSelector(std::move(selectable));
    m_layers = std::make_unique<GISLibrary::DatasetLayerSet>(*m_pLayerManager,
        [this](GISLibrary::DatasetLayerSet::DatasetId dataset) {
            return m_pLayerManager->CreateLayer(m_vecInventory[static_cast<std::size_t>(dataset)]->strFilePath);
        });
    // A cell is opened by the worker (DatasetOpening.h); its catalogues are loaded here first.
    m_layers->setPlanner([this](GISLibrary::DatasetLayerSet::DatasetId dataset) -> GISLibrary::DatasetLayerSet::Job {
        std::wstring path = m_vecInventory[static_cast<std::size_t>(dataset)]->strFilePath;
        if (!GISLibrary::OpensInBackground(path) || !m_pLayerManager->catalogManager)
        {
            return {};
        }
        const int product = m_pLayerManager->pathToProductNumber(path);
        if (!m_pLayerManager->catalogManager->getFC(product))
        {
            return {};
        }
        (void)m_pLayerManager->catalogManager->getPC(product);
        return [drawer = m_pLayerManager, path = std::move(path)]() { return drawer->CreateLayer(path); };
    });

	return true;
}

bool S100ExchangeCatalogue::OpenMetadata(const std::wstring& /*_filepath*/)
{
    return true;
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

std::vector<std::shared_ptr<InventoryItem>> S100ExchangeCatalogue::ItemsOf(std::span<const s100::sdb::SelectedCoverage> selection) const
{
    std::vector<std::shared_ptr<InventoryItem>> items;
    const auto coverages = m_selector.coverages();
    for (const auto& selected : selection)
    {
        const auto [inventoryIndex, coverageIndex] = m_coverageOwners[static_cast<std::size_t>(selected.coverage - coverages.data())];
        const auto& inventory = m_vecInventory[inventoryIndex];
        if (std::any_of(items.begin(), items.end(), [&](const auto& item) { return item->strFilePath == inventory->strFilePath; }))
        {
            continue;   // another coverage of a dataset already selected
        }
        auto item = std::make_shared<InventoryItem>();
        item->strFileName = inventory->strFileName;
        item->strFilePath = inventory->strFilePath;
        item->mbrBoundingBox = inventory->mbrBoundingBox;
        if (coverageIndex < inventory->vecBoundingPolygon.size()) item->BoundingPolygon = inventory->vecBoundingPolygon[coverageIndex];
        if (coverageIndex < inventory->vecScaleRange.size()) item->ScaleRange = inventory->vecScaleRange[coverageIndex];
        items.push_back(std::move(item));
    }
    return items;
}

void S100ExchangeCatalogue::DrawFiles(Scaler* scaler, std::vector<std::shared_ptr<InventoryItem>>& item)
{
    item.clear();
    const auto view = GISLibrary::selectionView(*scaler);
    if (!view || !m_layers)
    {
        return;
    }
    const auto selection = m_selector.select(*view);
    const auto datasets = s100::sdb::DatasetSelector::datasetsOf(selection);
    m_layers->show(datasets);
    item = ItemsOf(selection);
}

namespace
{
    // The largest font from `largest` down to `smallest`, a pixel at a time, in which a line is
    // no wider and no taller than a box; 0 if none is.
    float FittingFontSize(s100::render::IRenderer& renderer, const std::string& text, float largest, float smallest,
        float width, float height)
    {
        for (float size = largest; size >= smallest; size -= 1.0f)
        {
            s100::render::TextFont font;
            font.size = size;
            const s100::render::SizeF box = renderer.MeasureText(text, font);
            if (box.width <= width && box.height <= height)
            {
                return size;
            }
        }
        return 0.0f;
    }
}

void S100ExchangeCatalogue::Draw(GISLibrary::ViewFrame& frame, Scaler* scaler, double offset)
{
    std::vector<std::shared_ptr<InventoryItem>> items;

    DrawFiles(scaler, items);

    m_pLayerManager->Draw(frame, static_cast<int>(offset));

    // The overlay, over the datasets.
    if (frame.Begin(*scaler))
    {
        if (auto* renderer = frame.Renderer(nullptr))
        {
            DrawOverlay(*renderer, *scaler, items);
        }
        frame.End();
    }
}

// The box of every S-101 dataset shown, with its file name and the scales of its data
// coverages inside the box when they fit on a line.
void S100ExchangeCatalogue::DrawOverlay(s100::render::IRenderer& renderer, Scaler& scaler,
    const std::vector<std::shared_ptr<InventoryItem>>& items)
{
    const s100::render::ColorF red{ 1.0f, 0.0f, 0.0f, 1.0f };
    const s100::render::ColorF black{ 0.0f, 0.0f, 0.0f, 1.0f };
    renderer.SetTransform({});

    for (const auto& item : items)
    {
        for (auto& ddm : m_DataPtr->DatasetDiscoveryMetadata)
        {
            if (item->strFilePath != FixFileName(ddm.FileName))
            {
                continue;
            }
            if (std::string::npos == ddm.productSpecification->ProductIdentifier.find("101"))
            {
                break;
            }

            // The bounding box on the screen.
            const auto bb = ddm.BoundingBox;
            const auto onScreen = [&scaler](double lon, double lat) {
                projection(lon, lat);
                s100::render::PointF point;
                scaler.WorldToDevice_F(lon, lat, &point.x, &point.y);
                return point;
            };
            const s100::render::PointF topLeft = onScreen(bb->WestBoundLongitude, bb->NorthBoundLatitude);
            const s100::render::PointF bottomRight = onScreen(bb->EastBoundLongitude, bb->SouthBoundLatitude);
            s100::render::PathFigure box;
            box.start = topLeft;
            box.lines = { onScreen(bb->EastBoundLongitude, bb->NorthBoundLatitude), bottomRight,
                onScreen(bb->WestBoundLongitude, bb->SouthBoundLatitude) };
            box.closed = true;
            s100::render::Path path;
            path.figures.push_back(std::move(box));
            s100::render::Stroke stroke;
            stroke.width = 1.0f;
            renderer.StrokePath(path, red, stroke);

            const float rectWidth = std::fabs(bottomRight.x - topLeft.x);
            const float rectHeight = std::fabs(bottomRight.y - topLeft.y);
            const float left = (std::min)(topLeft.x, bottomRight.x);
            const float top = (std::min)(topLeft.y, bottomRight.y);

            // The file name at the top left, as large as fits on one line: at most 50 pixels
            // and three tenths of the height of the box.
            const std::string fileName = "FileName : " + std::string(ddm.FileName);
            const float fontSize = FittingFontSize(renderer, fileName, (std::min)(50.0f, rectHeight * 0.3f), 10.0f,
                rectWidth * 0.95f, rectHeight * 0.3f);
            if (fontSize <= 0.0f)
            {
                break;
            }
            s100::render::TextFont font;
            font.size = fontSize;
            renderer.FillText(fileName, { left, top }, font, black);

            // Under it, the scales of each data coverage at six tenths of that size.
            const float textSpacing = fontSize * 0.2f;
            float currentY = top + fontSize * 1.2f + textSpacing;
            for (const auto& dc : ddm.dataCoverage)
            {
                const std::string optimum = "Opt Scale: " + (dc.OptimumDisplayScale ? std::to_string(*dc.OptimumDisplayScale) : std::string("-"));
                const std::string minimum = "Min Scale: " + (dc.MinimumDisplayScale ? std::to_string(*dc.MinimumDisplayScale) : std::string("-"));
                const std::string& longer = optimum.size() > minimum.size() ? optimum : minimum;
                const float scaleSize = FittingFontSize(renderer, longer, std::clamp(fontSize * 0.6f, 10.0f, 30.0f), 10.0f,
                    rectWidth * 0.95f, rectHeight);
                if (scaleSize <= 0.0f)
                {
                    continue;
                }
                s100::render::TextFont scaleFont;
                scaleFont.size = scaleSize;
                renderer.FillText(optimum, { left, currentY }, scaleFont, black);
                currentY += scaleSize * 1.2f + textSpacing;
                renderer.FillText(minimum, { left, currentY }, scaleFont, black);
                currentY += scaleSize * 1.2f + textSpacing;
            }
            break;   // the dataset of the item is found
        }
    }
}
