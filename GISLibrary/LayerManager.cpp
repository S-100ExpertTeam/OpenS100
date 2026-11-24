#include "stdafx.h"
#include "SkiaViewFrame.h"
#include "FeatureHighlight.h"
#include "PathUtil.h"
#include "TextCompare.h"
#include "LayerManager.h"
#include "LayerOpen.h"
#include "PortrayalRenderCache.h"
#include "BinaryFile.h"
#include <filesystem>
#include "F_ATTR.h"
#include "R_FeatureRecord.h"
#include "ATTR.h"
#include "CodeWithNumericCode.h"
#include "DrawingSet.h"
#include "ProcessS101.h"
#include "PCOutputSchemaManager.h"
#include "SENC_SpatialReference.h"
#include "SENC_DisplayList.h"
#include "SENC_Instruction.h"
#include "SENC_LineInstruction.h"
#include "SENC_PointInstruction.h"
#include "SENC_TextInstruction.h"
#include "SENC_AlertReference.h"
#include "SENC_Define.h"
#include "DRReader.h"
#include "S100Utilities.h"
#include "GISLibrary.h"
#include "F_FASC.h"
#include "F_INAS.h"
#include "R_InformationRecord.h"
#include "S100Layer.h"
#include "SSurface.h"
#include "SMultiPoint.h"
#include "SCompositeCurve.h"
#include "SPoint.h"
#include "SCommonFuction.h"
#include "CoverageDataset.h"
#include "DrawingOrder.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../GeoMetryLibrary/GeometricFuc.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include "../LatLonUtility/LatLonUtility.h"
#include "../LatLonUtility/cpp_util.h"

#include <ctime> 
#include <mmsystem.h> 
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <future>
#include <chrono>
#include <memory>
#include "S100ExchangeCatalogue.h"
#include "LayerContainer.h"

LayerManager::LayerManager()
{
}

LayerManager::LayerManager(Scaler* scaler, CatalogManager* catalogManager) : LayerManager()
{
	this->scaler = scaler;
	this->catalogManager = catalogManager;
}

LayerManager::~LayerManager()
{
	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		delete* i;
	}
	layers.clear();

	featureOnOffMap.clear();
}

void LayerManager::MoveLayerFromList(int from, int to) {
	if (from == to) return;

	auto fromIt = std::next(layers.begin(), from);
	auto toIt = std::next(layers.begin(), to);

	layers.splice(toIt, layers, fromIt);
}

bool LayerManager::IsContainFilePathToLayer(const std::wstring& _filepath)
{
	for (auto layer : layers) {
		if (layer->GetLayerPath() == _filepath) {
			return true;
		}
	}
	return false;
}

bool LayerManager::AddBackgroundLayer(const std::wstring& _filepath)
{
	const std::wstring file_extension = s100::path::Extension(_filepath);

	if (s100::text::CompareNoCase(file_extension, L"SHP") == 0) 
	{
		if (GISLibrary::OpenLayer(backgroundLayer, _filepath, this) == false)
		{
			return false;
		}
	}

	double xmin = scaler->mxMinLimit;
	double xmax = scaler->mxMaxLimit;
	double ymin = scaler->myMinLimit;
	double ymax = scaler->myMaxLimit;

	MBR _mbr(xmin, ymin, xmax, ymax);
	if (m_isScreenFitEnabled)
		scaler->SetMap(_mbr);
	mbr.SetMBR(_mbr);

	return true;
}

int LayerManager::AddLayer(Layer* _layer , const std::wstring& FilePath, bool InsertBack)
{
	if (_layer == nullptr)
	{
		return -1;
	}

	_layer->SetID(CreateLayerID());

	if (LayerCount() == 0)
	{
		MBR newMBR = _layer->GetMBR();
		if (newMBR.IsEmpty())
		{
			newMBR.Extent(0.001);
		}

		mbr.SetMBR(newMBR);
		if (m_isScreenFitEnabled)
			scaler->SetMap(mbr);
	}
	else
	{
		mbr.CalcMBR(_layer->m_mbr);
	}

	if (FilePath.empty())
		layers.push_back(_layer);
	else
	{
		for (auto it = layers.begin(); it != layers.end(); ++it) {
			if ((*it)->GetLayerPath() == FilePath)
			{
				if (InsertBack) {
					it++;
					layers.insert(it, _layer);
					break;
				}
				else
				{
					layers.insert(it, _layer);
					break;
				}

			}
		}
	}


	mapLayer.insert({ _layer->GetID(), _layer });

	return _layer->GetID();
}

int LayerManager::AddLayer(Layer* _layer)
{
	if (_layer == nullptr)
	{
		return -1;
	}

	_layer->SetID(CreateLayerID());

	if (LayerCount() == 0)
	{
		MBR newMBR = _layer->GetMBR();
		if (newMBR.IsEmpty())
		{
			newMBR.Extent(0.001);
		}

		mbr.SetMBR(newMBR);
		if (m_isScreenFitEnabled)
			scaler->SetMap(mbr);
	}
	else
	{
		mbr.CalcMBR(_layer->m_mbr);
	}
	
	layers.push_back(_layer);
	mapLayer.insert({ _layer->GetID(), _layer });

	return _layer->GetID();
}


int LayerManager::AddLayerFront(Layer* _layer)
{
	if (_layer == nullptr)
	{
		return -1;
	}

	_layer->SetID(CreateLayerID());

	if (LayerCount() == 0)
	{
		MBR newMBR = _layer->GetMBR();
		if (newMBR.IsEmpty())
		{
			newMBR.Extent(0.001);
		}

		mbr.SetMBR(newMBR);
		if (m_isScreenFitEnabled)
			scaler->SetMap(mbr);
	}
	else
	{
		mbr.CalcMBR(_layer->m_mbr);
	}

	layers.push_front(_layer);
	mapLayer.insert({ _layer->GetID(), _layer });

	return _layer->GetID();
}

bool LayerManager::AddUpdateLayer(Layer* /*Base*/, Layer* /*Update*/)
{

	return false;
}

int LayerManager::isUpdate(const std::wstring& filePath)
{
	const std::wstring file_extension = s100::path::Extension(filePath);

	try
	{
		int value = static_cast<int>(std::wcstol(file_extension.c_str(), nullptr, 10));
		return value;
	}
	catch (std::exception e)
	{
		return 0;
	}

	return 0;
}

std::unique_ptr<Layer> LayerManager::CreateS100Layer(const std::wstring& _filepath, int productNumber, FeatureCatalogue* fc, PortrayalCatalogue* pc)
{
	if (!fc)
	{
		return nullptr;
	}
	auto layer = std::make_unique<S100Layer>(productNumber, fc, pc);
	if (GISLibrary::OpenLayer(*layer, _filepath, this) == false || !layer->m_spatialObject)
	{
		return nullptr;
	}
	BuildPortrayalCatalogue(layer.get());
	return layer;
}

std::unique_ptr<Layer> LayerManager::CreateLayer(const std::wstring& _filepath)
{
	std::unique_ptr<Layer> layer;

	S100_FileType fileType = CheckFileType(_filepath);

	if (fileType == S100_FileType::FILE_Shape ||
		fileType == S100_FileType::FILE_ETC)
	{
		layer = std::make_unique<Layer>();
		if (GISLibrary::OpenLayer(*layer, _filepath, this) == false)
		{
			return nullptr;
		}
	}
	else if (fileType == S100_FileType::FILE_S_100_VECTOR ||
		fileType == S100_FileType::FILE_S_100_GRID_H5)
	{
		auto productNumber = pathToProductNumber(_filepath);
		layer = CreateS100Layer(_filepath, productNumber, catalogManager->getFC(productNumber), catalogManager->getPC(productNumber));
	}

	if (!layer || !layer->m_spatialObject)
	{
		return nullptr;
	}
	return layer;
}

int LayerManager::AddLayer(const std::wstring& _filepath)
{
	Layer* layer = CreateLayer(_filepath).release();
	if (!layer)
	{
		return -1;
	}

	auto tempTypePtr = dynamic_cast<S100ExchangeCatalogue*>(layer->m_spatialObject);
	if (tempTypePtr != nullptr)
	{
		const std::wstring firstlayerfilepath = tempTypePtr->GetFirstLayerFilePath();
		AddLayer(layer, firstlayerfilepath, false);
	}
	else
	{
		AddLayer(layer);
	}

	return layer->GetID();
}


int LayerManager::AddLayerFront(const std::wstring& _filepath)
{
	Layer* layer = nullptr;

	S100_FileType fileType = CheckFileType(_filepath);

	if (fileType == S100_FileType::FILE_Shape ||
		fileType == S100_FileType::FILE_ETC)
	{
		layer = new Layer();
		if (GISLibrary::OpenLayer(*layer, _filepath, this) == false)
		{
			delete layer;
			return -1;
		}
	}
	else if (fileType == S100_FileType::FILE_S_100_VECTOR ||
		fileType == S100_FileType::FILE_S_100_GRID_H5)
	{
		auto productNumber = pathToProductNumber(_filepath);
		auto fc = catalogManager->getFC(productNumber);
		auto pc = catalogManager->getPC(productNumber);

		if (fc)
		{
			layer = new S100Layer(fc, pc);
			if (GISLibrary::OpenLayer(*layer, _filepath, this) == false)
			{
				delete layer;
				return -1;
			}

			BuildPortrayalCatalogue(layer);
		}
	}

	if (!layer ||
		!layer->m_spatialObject)
	{
		if (layer)
		{
			delete layer;
		}

		return -1;
	}


	AddLayerFront(layer);

	return layer->GetID();
}


void LayerManager::Draw(HDC& hdc, int offset)
{
	// Everything a refresh draws is one scene: the layers are rasterised together and put on
	// the device context once.
	GISLibrary::SkiaViewFrame::Scene scene(GISLibrary::SkiaViewFrame::Shared(), hdc);

	DrawBackground(hdc, offset);

	// Layer containers (the System Database chart, exchange sets) are base charts: drawn under
	// the datasets opened on their own.
	DrawLayerContainers(hdc, offset);

	DrawS100Datasets(hdc, offset);

	DrawNonS100Datasets(hdc, offset);

	//gisLib->D2.Begin(hdc, rectView);
	//gisLib->DrawS100Symbol(101, L"NORTHAR1", 30, 50, 0);
	//gisLib->DrawScaleBar();
	//gisLib->D2.End();
}

void LayerManager::DrawInformationLayer(HDC& hDC, Layer* layer)
{
	if (nullptr == layer)
	{
		return;
	}

	if (false == MBR::CheckOverlap(scaler->GetMap(), layer->m_mbr))
	{
		return;
	}

	auto _mbr = layer->GetMBR();

	// Internal coordinate system -> Screen coordinate system.
	long sxmin = 0;
	long symax = 0;
	scaler->WorldToDevice(_mbr.xmin, _mbr.ymin, &sxmin, &symax);
	long sxmax = 0;
	long symin = 0;
	scaler->WorldToDevice(_mbr.xmax, _mbr.ymax, &sxmax, &symin);

	auto& frame = GISLibrary::SkiaViewFrame::Shared();
	if (!frame.Begin(*scaler))
	{
		return;
	}
	if (auto* renderer = frame.Renderer(nullptr))
	{
		GISLibrary::DrawInformationBox(*renderer, static_cast<float>(sxmin), static_cast<float>(symin), static_cast<float>(sxmax), static_cast<float>(symax),
			"LayerName : " + pugi::as_utf8(layer->GetLayerName()), "LayerType : " + pugi::as_utf8(layer->GetLayerType()));
	}
	frame.End(hDC);
}

// The instructions of the layer the view shows, into its drawing set.
void LayerManager::CollectInstructions(S100Layer* layer)
{
	auto cell = (S100SpatialObject*)layer->GetSpatialObject();
	// With no feature type turned off there is nothing to ask of an instruction.
	s100::render::FeatureFilter shown;
	if (!featureOnOffMap.empty())
	{
		shown = [this](const std::wstring& code) { return IsFeatureOn(code); };
	}
	s100::render::CollectInstructions(*cell->pcManager->displayListSENC, GetS100Scale(), *scaler, shown,
		layer->drawingSet, layer->drawingPriority);
}


void LayerManager::DrawBackground(HDC &hDC, int offset)
{
	if (m_baseMapOn)
	{
		if (backgroundLayer.IsOn())
		{
			if (MBR::CheckOverlap(scaler->GetMap(), backgroundLayer.m_mbr))
			{
				// On Mercator the world goes round: the copies a turn of longitude either side.
				if (!IsPolarProjection())
				{
					backgroundLayer.Draw(hDC, scaler, offset - 360);
				}
				backgroundLayer.Draw(hDC, scaler, offset);
				if (!IsPolarProjection())
				{
					backgroundLayer.Draw(hDC, scaler, offset + 360);
				}
			}
		}
	}
}

void LayerManager::DrawS100Datasets(HDC& hdc, int offset)
{
	if (onIC == false)
	{
		// Consecutive layers of one draw group are drawn together, when the group ends.
		std::vector<S100Layer*> group;
		int groupKey = 0;
		const auto drawGroup = [&]() {
			if (group.size() == 1)
			{
				DrawS100Layer(hdc, offset, group.front());
			}
			else if (group.size() > 1)
			{
				DrawS100Layers(hdc, offset, group);
			}
			group.clear();
		};

		for (auto i = layers.begin(); i != layers.end(); i++)
		{
			auto layer = (*i);

			if (layer->IsOn())
			{
				if (layer->IsS100Layer())
				{
					if (layer->GetFileType() == S100_FileType::FILE_S_100_VECTOR)
					{
						const auto key = drawGroups.find(layer);
						if (key == drawGroups.end() || (!group.empty() && key->second != groupKey))
						{
							drawGroup();
						}
						if (key == drawGroups.end())
						{
							DrawS100Layer(hdc, offset, (S100Layer*)layer);
						}
						else
						{
							groupKey = key->second;
							group.push_back((S100Layer*)layer);
						}
					}
					else if (layer->GetFileType() == S100_FileType::FILE_S_100_GRID_H5)
					{
						drawGroup();
						// A grid without a renderer of its own shows where it lies.
						if (!static_cast<S100Layer*>(layer)->DrawGrid(hdc, scaler))
						{
							DrawInformationLayer(hdc, layer);
						}
					}
				}
			}
		}
		drawGroup();
	}
	else
	{
		for (auto i = layers.begin(); i != layers.end(); i++)
		{
			auto layer = (*i);

			if (layer->IsOn())
			{
				if (layer->IsS100Layer())
				{
					if (layer->GetFileType() == S100_FileType::FILE_S_100_VECTOR)
					{
						SetDrawingInstruction((S100Layer*)layer);
					}
				}
			}
		}

		for (auto i = layers.begin(); i != layers.end(); i++)
		{
			auto layer = (*i);

			if (layer->IsOn())
			{
				if (layer->IsS100Layer())
				{
					if (layer->GetFileType() == S100_FileType::FILE_S_100_VECTOR)
					{
						DrawS100Layer(hdc, offset, (S100Layer*)layer, 0, 3);
					}
				}
			}
		}

		for (auto i = layers.begin(); i != layers.end(); i++)
		{
			auto layer = (*i);

			if (layer->IsOn())
			{
				if (layer->IsS100Layer())
				{
					if (layer->GetFileType() == S100_FileType::FILE_S_100_GRID_H5)
					{
						static_cast<S100Layer*>(layer)->DrawGrid(hdc, scaler);
					}
				}
			}
		}

		for (auto i = layers.begin(); i != layers.end(); i++)
		{
			auto layer = (*i);

			if (layer->IsOn())
			{
				if (layer->IsS100Layer())
				{
					if (layer->GetFileType() == S100_FileType::FILE_S_100_VECTOR)
					{
						DrawS100Layer(hdc, offset, (S100Layer*)layer, 4, 100);
					}
				}
			}
		}

		for (auto i = layers.begin(); i != layers.end(); i++)
		{
			auto layer = (*i);

			if (layer->IsOn())
			{
				if (layer->IsS100Layer())
				{
					if (layer->GetFileType() == S100_FileType::FILE_S_100_VECTOR)
					{
						auto s100Layer = (S100Layer*)layer;
						s100Layer->InitDraw();
					}
				}
			}
		}
	}
}

void LayerManager::DrawECDISMode(HDC& hDC, int offset)
{
	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		auto layer = (*i);

		if (!layer->IsS100Layer())
		{
			layer->Draw(hDC, scaler, offset);
		}
	}
}

void LayerManager::DrawLayerContainers(HDC& hDC, int offset)
{
	for (auto layer : layers)
	{
		if (dynamic_cast<GISLibrary::LayerContainer*>(layer->m_spatialObject))
		{
			layer->Draw(hDC, scaler, offset);
		}
	}
}

void LayerManager::DrawNonS100Datasets(HDC& hDC, int offset)
{
	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		auto layer = (*i);

		if (!layer->IsS100Layer() && !dynamic_cast<GISLibrary::LayerContainer*>(layer->m_spatialObject))
		{
			layer->Draw(hDC, scaler, offset);
		}
	}
}

void LayerManager::DrawS100Layer(HDC& hDC, int /*offset*/, S100Layer* layer)
{
	auto pc = layer->GetPC();
	layer->InitDraw();

	auto cell = (S101Cell*)layer->GetSpatialObject();
	if (nullptr == cell->pcManager ||
		nullptr == cell->pcManager->displayListSENC)
	{
		return;
	}

	if (false == MBR::CheckOverlap(scaler->GetMap(), layer->m_mbr))
	{
		return;
	}

	CollectInstructions(layer);

	// Line Suppression
	s100::render::SuppressSharedCurves(layer->drawingPriority, layer->drawingSet);

	auto& frame = GISLibrary::SkiaViewFrame::Shared();
	if (frame.Begin(*scaler))
	{
		for (const int priority : layer->drawingPriority)
		{
			frame.Paint(priority, layer->drawingSet, pc, *scaler, ENCCommon::TEXTOUT);
		}
		frame.End(hDC);
	}
}

void LayerManager::SetDrawingInstruction(S100Layer* layer)
{

	auto cell = (S100SpatialObject*)layer->GetSpatialObject();
	if (nullptr == cell->pcManager ||
		nullptr == cell->pcManager->displayListSENC)
	{
		return;
	}

	if (false == MBR::CheckOverlap(scaler->GetMap(), layer->m_mbr))
	{
		return;
	}

	CollectInstructions(layer);

	// Line Suppression
	if (101 == layer->GetProductNumber()) {
		s100::render::SuppressSharedCurves(layer->drawingPriority, layer->drawingSet);
	}
}

void LayerManager::DrawS100Layer(HDC& hDC, int /*offset*/, S100Layer* layer, int minPriority, int maxPriority)
{
	if (false == MBR::CheckOverlap(scaler->GetMap(), layer->m_mbr)) {
		return;
	}

	auto pc = layer->GetPC();
	if (nullptr == pc)
	{
		return;
	}

	auto& frame = GISLibrary::SkiaViewFrame::Shared();
	if (frame.Begin(*scaler))
	{
		for (const int priority : layer->drawingPriority)
		{
			if (priority < minPriority || priority > maxPriority)
			{
				continue;
			}
			frame.Paint(priority, layer->drawingSet, pc, *scaler, ENCCommon::TEXTOUT);
		}
		frame.End(hDC);
	}
}

void LayerManager::DrawS100Layers(HDC& hDC, int /*offset*/, const std::vector<S100Layer*>& group)
{
	std::set<int> priorities;
	std::vector<PortrayalCatalogue*> catalogues;
	for (S100Layer* layer : group)
	{
		layer->InitDraw();
		auto pc = layer->GetPC();
		if (!pc)
		{
			continue;
		}
		SetDrawingInstruction(layer);
		priorities.insert(layer->drawingPriority.begin(), layer->drawingPriority.end());
		if (!layer->drawingPriority.empty() && std::find(catalogues.begin(), catalogues.end(), pc) == catalogues.end())
		{
			catalogues.push_back(pc);
		}
	}
	if (priorities.empty())
	{
		return;
	}

	auto& frame = GISLibrary::SkiaViewFrame::Shared();
	if (frame.Begin(*scaler))
	{
		for (const int priority : priorities)
		{
			for (S100Layer* layer : group)
			{
				if (layer->drawingPriority.contains(priority))
				{
					frame.Paint(priority, layer->drawingSet, layer->GetPC(), *scaler, ENCCommon::TEXTOUT);
				}
			}
		}
		frame.End(hDC);
	}
}

void LayerManager::S101RebuildPortrayal()
{
	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		auto layer = (*i);

		if (auto container = dynamic_cast<GISLibrary::LayerContainer*>(layer->m_spatialObject))
		{
			container->DropCachedLayers();
			if (auto inner = container->GetInnerLayers())
			{
				inner->S101RebuildPortrayal();
			}
			continue;
		}

		if (layer->m_spatialObject->m_FileType == S100_FileType::FILE_S_100_VECTOR)
		{
			BuildPortrayalCatalogue(layer);
		}
		else if (layer->m_spatialObject->m_FileType == S100_FileType::FILE_S_100_GRID_H5)
		{
			BuildPortrayalCatalogue(layer);
		}
		else if (layer->m_spatialObject->m_FileType == S100_FileType::FILE_S_100_GRID_BAG)
		{
			BuildPortrayalCatalogue(layer);
		}
	}
}

void LayerManager::BuildPortrayalCatalogue(Layer* l)
{
	auto pc = ((S100Layer*)l)->GetPC();

	// Gridded coverages portrayed by their catalogue's rules (S-102).
	if (auto coverage = dynamic_cast<GISLibrary::CoverageDataset*>(l->GetSpatialObject()))
	{
		coverage->Portray(((S100Layer*)l)->GetFC(), pc);
		return;
	}

	if (pc &&
		l->GetFileType() == S100_FileType::FILE_S_100_VECTOR) 
	{
		auto mainRuleFile = pc->GetMainRuleFile();
		auto fileName = mainRuleFile->GetFileNameW();
		auto rootPath = pc->GetRootPathW();
		auto mainRulePath = rootPath + L"Rules\\" + fileName;

		if (pc->GetRuleFileFormat() == Portrayal::FileFormat::LUA) 
		{
			ProcessS101::ProcessS101_LUA(mainRulePath, (S100Layer*)l);
		}
		else if (pc->GetRuleFileFormat() == Portrayal::FileFormat::XSLT) 
		{
			auto gml = (S10XGML*)l->GetSpatialObject();
			gml->SaveToInputXML("..\\TEMP\\input.xml");
			ProcessS101::ProcessS100_XSLT("..\\TEMP\\input.xml", pugi::as_utf8(mainRulePath), "..\\TEMP\\output.xml");
			auto s100so = (S100SpatialObject*)l->GetSpatialObject();
			s100so->OpenOutputXML("..\\TEMP\\output.xml");
		}
	}
}

Layer* LayerManager::GetLayer(int index)
{
	if (index < 0 || index >= LayerCount())
	{
		return nullptr;
	}

	auto it = layers.begin();

	std::advance(it, index);

	return *it;
}

Layer* LayerManager::GetLayerByKey(int key)
{
	auto item = mapLayer.find(key);
	if (item == mapLayer.end())
	{
		return nullptr;
	}

	return item->second;
}

std::wstring LayerManager::GetLayerName(int index)
{
	auto layer = GetLayer(index);

	if (layer)
	{
		return layer->GetLayerName();
	}
	return L"";
}

std::wstring LayerManager::GetLayerNameByKey(int key)
{
	auto layer = GetLayerByKey(key);

	if (layer)
	{
		return layer->GetLayerName();
	}
	return L"";
}

bool LayerManager::IsOn(int index)
{
	auto layer = GetLayer(index);

	if (layer)
	{
		return layer->IsOn();
	}

	return false;
}

bool LayerManager::IsOnByKey(int key)
{
	auto layer = GetLayerByKey(key);

	if (layer)
	{
		return layer->IsOn();
	}

	return false;
}

void LayerManager::DeleteLayer(int index)
{
	if (index < 0 || index >= LayerCount())
	{
		return;
	}

	auto it = layers.begin();

	std::advance(it, index);

	auto id = (*it)->GetID();

	delete (*it);

	layers.erase(it);
	mapLayer.erase(id);

	ReMBR();

	return;
}

std::unique_ptr<Layer> LayerManager::ReleaseLayer(Layer* layer)
{
	const auto found = std::find(layers.begin(), layers.end(), layer);
	if (found == layers.end())
	{
		return nullptr;
	}
	layers.erase(found);
	mapLayer.erase(layer->GetID());
	drawGroups.erase(layer);
	ReMBR();
	return std::unique_ptr<Layer>(layer);
}

void LayerManager::FindPathDeleteLayer(const std::wstring& FindLayerPath)
{
	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		auto layer = *i;
		if (layer->GetLayerPath().find(FindLayerPath) != std::wstring::npos)
		{
			mapLayer.erase(layer->GetID());
			delete layer;
			layer = nullptr;
			layers.erase(i);
			ReMBR();
			return;
		}
	}
}

void LayerManager::DeleteLayer(const std::wstring& filepath)
{
	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		auto layer = *i;
		if (layer->GetLayerPath() == filepath)
		{
			mapLayer.erase(layer->GetID());
			delete layer;
			layer = nullptr;
			layers.erase(i);
			ReMBR();
			return;
		}
	}
}

void LayerManager::DeleteLayerByKey(int key)
{
	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		auto layer = (*i);
		if (layer->GetID() == key)
		{
			mapLayer.erase(layer->GetID());
			delete layer;
			layer = nullptr;
			layers.erase(i);
			ReMBR();
			return;
		}
	}
}

void LayerManager::ReMBR()
{
	if (LayerCount() == 0)
	{
		// The world between 30 degrees south and north; about a pole, the hemisphere.
		mbr = IsPolarProjection() ? ProjectionLimits() : MBR(-170.0, -30.0, 170.0, 30.0);
		if (!IsPolarProjection())
		{
			projection(mbr);
		}
	}
	else
	{
		mbr.InitMBR();

		for (auto i = layers.begin(); i != layers.end(); i++)
		{
			mbr.CalcMBR((*i)->GetMBR());
		}
	}
}

void LayerManager::ReloadLayers()
{
	if (backgroundLayer.m_spatialObject)
	{
		const std::wstring path = backgroundLayer.GetLayerPath();
		delete backgroundLayer.m_spatialObject;
		backgroundLayer.m_spatialObject = nullptr;
		GISLibrary::OpenLayer(backgroundLayer, path, this);
	}

	struct Opened
	{
		std::wstring path;
		bool on = true;
		bool info = false;
	};
	std::vector<Opened> opened;
	for (auto it = layers.begin(); it != layers.end();)
	{
		Layer* layer = *it;
		auto container = layer->m_spatialObject ? dynamic_cast<GISLibrary::LayerContainer*>(layer->m_spatialObject) : nullptr;
		if (container && container->ReloadLayers())
		{
			++it;
			continue;
		}
		if (layer->m_spatialObject)
		{
			opened.push_back({ layer->GetLayerPath(), layer->On, layer->Info });
		}
		mapLayer.erase(layer->GetID());
		delete layer;
		it = layers.erase(it);
	}

	// The view is left to the caller. An exchange catalogue opens its datasets as layers of
	// their own: those are not opened a second time.
	const bool fit = m_isScreenFitEnabled;
	m_isScreenFitEnabled = false;
	for (const auto& file : opened)
	{
		if (!IsContainFilePathToLayer(file.path))
		{
			AddLayer(file.path);
		}
	}
	m_isScreenFitEnabled = fit;

	for (Layer* layer : layers)
	{
		if (!layer->m_spatialObject)
		{
			continue;
		}
		const std::wstring path = layer->GetLayerPath();
		for (const auto& file : opened)
		{
			if (file.path == path)
			{
				layer->On = file.on;
				layer->Info = file.info;
				break;
			}
		}
	}
	ReMBR();
}

void LayerManager::SetViewMBR(RECT r)
{
	scaler->SetMapFromScreen(r.left, r.top, r.right, r.bottom);
}

void LayerManager::ChangeS100ColorPalette(GeoMetryLibrary::ColorTable value)
{
	if (value == GeoMetryLibrary::ColorTable::Day)
	{
		ChangeS100ColorPalette(L"Day");
	}
	else if (value == GeoMetryLibrary::ColorTable::Dusk)
	{
		ChangeS100ColorPalette(L"Dusk");
	}
	else if (value == GeoMetryLibrary::ColorTable::Night)
	{
		ChangeS100ColorPalette(L"Night");
	}
}

void LayerManager::ChangeS100ColorPalette(std::wstring paletteName)
{
	ChangeLayerPalettes(paletteName);

	for (auto pc : catalogManager->PCs)
	{
		PortrayalRender::ChangePalette(pc, paletteName);
	}
}

void LayerManager::ChangeLayerPalettes(const std::wstring& paletteName)
{
	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		auto layer = *i;
		if (auto container = layer ? dynamic_cast<GISLibrary::LayerContainer*>(layer->m_spatialObject) : nullptr)
		{
			container->DropCachedLayers();
			if (auto inner = container->GetInnerLayers())
			{
				inner->ChangeLayerPalettes(paletteName);
			}
		}
		else if (layer && true == layer->IsS100Layer())
		{
			auto s100layer = (S100Layer*)layer;
			auto pc = s100layer->GetPC();
			if (nullptr != pc)
			{
				PortrayalRender::ChangePalette(pc, paletteName);
			}
		}
	}
}

Scaler* LayerManager::GetScaler()
{
	return scaler;
}
/*
*/
int LayerManager::LayerCount()
{
	return (int)layers.size();
}

namespace
{
	// Looks at the first data record of an ISO 8211 file: an S-57 cell starts with field
	// "0001", an S-100 dataset with "DSID". Platform-neutral (no CFile).
	S100_FileType SniffIso8211(const std::filesystem::path& path)
	{
		constexpr std::size_t headerBytes = 1024;
		auto bytes = s100::io::ReadBytes(path, headerBytes);
		if (!bytes || bytes->size() < headerBytes)
		{
			return S100_FileType::FILE_NONE;
		}

		std::uint8_t* cursor = bytes->data();
		DRReader drReader;
		drReader.ReadReader(cursor);
		const std::size_t tagOffset = static_cast<std::size_t>(cursor - bytes->data()) + drReader.m_fieldPosition + drReader.m_fieldLength + 4;
		if (tagOffset + 4 > bytes->size())
		{
			return S100_FileType::FILE_NONE;
		}

		const std::string tag(reinterpret_cast<const char*>(bytes->data() + tagOffset), 4);
		if (tag == "0001")
		{
			return S100_FileType::FILE_S_57;
		}
		if (tag == "DSID")
		{
			return S100_FileType::FILE_S_100_VECTOR;
		}
		return S100_FileType::FILE_NONE;
	}
}

S100_FileType LayerManager::CheckFileType(const std::wstring& path, int update)
{
	S100_FileType ret = S100_FileType::FILE_NONE;

	const std::wstring file_extension = s100::path::Extension(path);
	auto isUpdatFile = update > 0 ? true : false;

	if (s100::text::CompareNoCase(file_extension, L"000") == 0 || isUpdatFile)
	{
		ret = SniffIso8211(path);
	}
	else if (s100::text::CompareNoCase(file_extension, L"XML") == 0 || s100::text::CompareNoCase(file_extension, L"GML") == 0)
	{
		if (!path.empty())
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(path.c_str());
			if (!result)
			{
				return S100_FileType::FILE_NONE;
			}
			pugi::xml_node nodeList = doc.first_child();

			std::wstring nodeName = pugi::as_wide(nodeList.name());
			if ((nodeName.find(L"DataSet") != std::wstring::npos) ||
				(nodeName.find(L"Dataset") != std::wstring::npos))
			{
				ret = S100_FileType::FILE_S_100_VECTOR;
			}
		}
	}
	else if (s100::text::CompareNoCase(file_extension, L"h5") == 0)
	{
		ret = S100_FileType::FILE_S_100_GRID_H5;
	}

	return ret;
}

S100_FileType LayerManager::CheckFileType(const std::wstring& path)
{
	S100_FileType ret = S100_FileType::FILE_NONE;

	const std::wstring file_extension = s100::path::Extension(path);


	if ((s100::text::CompareNoCase(file_extension, L"000") >= 0) &&
		(s100::text::CompareNoCase(file_extension, L"999") <= 0))
	{
		auto sniffed = SniffIso8211(path);
		if (sniffed != S100_FileType::FILE_NONE)
		{
			return sniffed;
		}
	}
	else if (s100::text::CompareNoCase(file_extension, L"XML") == 0 || s100::text::CompareNoCase(file_extension, L"GML") == 0)
	{
		if (!path.empty())
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(path.c_str());
			if (!result)
			{
				return S100_FileType::FILE_NONE;
			}
			pugi::xml_node nodeList = doc.first_child();

			std::wstring nodeName = pugi::as_wide(nodeList.name());

			nodeName = LatLonUtility::To_Lowercase(nodeName);

			//if ((nodeName.find(L"DataSet") != std::wstring::npos) ||
			//	(nodeName.find(L"Dataset") != std::wstring::npos))
			if (nodeName.find(L"dataset") != std::wstring::npos)
			{
				return S100_FileType::FILE_S_100_VECTOR;
			}
			else
			{
				return S100_FileType::FILE_ETC;
			}
		}
	}
	else if (s100::text::CompareNoCase(file_extension, L"h5") == 0)
	{
		return S100_FileType::FILE_S_100_GRID_H5;
	}
	else if (s100::text::CompareNoCase(file_extension, L"SHP") == 0)
	{
		return S100_FileType::FILE_Shape;
	}

	return ret;
}

int LayerManager::CreateLayerID()
{
	std::set<int> IDs;

	for (auto i = layers.begin(); i != layers.end(); i++)
	{
		auto layer = (*i);
		auto id = layer->GetID();
		IDs.insert(id);
	}


	if (IDs.size() > 0)
	{
		for (int i = 1; i <= INT_MAX && i >= 0; i++)
		{
			auto item = IDs.find(i);
			if (item == IDs.end())
			{
				return i;
			}
		}
	}

	return 1;
}

int LayerManager::pathToProductNumber(const std::wstring& path)
{
	const auto name = s100::path::Stem(path);
	if (name.size() >= 3)
	{
		const std::wstring strProductNumber = name.substr(0, 3);

		try {
			return cpp_util::stoi(strProductNumber);
		}
		catch (const std::exception&) {
		}
	}

	return 0;
}

void LayerManager::SetS100Scale(double value)
{
	s100Scale = value;
}

int LayerManager::GetS100Scale()
{
	if (s100Scale > 0)
	{
		return (int)s100Scale;
	}

	auto currentScale = scaler->GetCurrentScale();

	return currentScale;
}

void LayerManager::InitFeatureOnOffMap()
{
	auto fc = catalogManager->getFC("S-101");
	if (fc)
	{
		featureOnOffMap.clear();

		auto vector = fc->GetFeatureTypes()->GetVecFeatureType();
		for (auto i = vector.begin(); i != vector.end(); i++)
		{
			featureOnOffMap.insert({ (*i)->GetCodeAsWString(), true });
		}
	}
}

void LayerManager::SetFeatureOnOff(std::wstring code, bool on)
{
	auto itor = featureOnOffMap.find(code);
	if (itor != featureOnOffMap.end())
	{
		itor->second = on;
	}
	else
	{
		featureOnOffMap.insert({ code, on });
	}
}

bool LayerManager::IsFeatureOn(const std::wstring& featureTypeCode)
{
	auto item = featureOnOffMap.find(featureTypeCode);
	if (item != featureOnOffMap.end())
	{
		return item->second;
	}

	return true;
}

