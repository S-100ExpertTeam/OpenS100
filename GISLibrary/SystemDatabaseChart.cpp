#include "stdafx.h"
#include "SystemDatabaseChart.h"

#include "CatalogManager.h"
#include "Layer.h"
#include "LayerManager.h"
#include "ViewSelection.h"

#include "../GeoMetryLibrary/GeoCommonFuc.h"
#include "../SystemDatabase/CatalogueResolution.h"
#include "../SystemDatabase/ResourceStore.h"
#include "../SystemDatabase/SystemDatabase.h"

#include <charconv>

namespace GISLibrary
{
	namespace
	{
		// "S-101" -> 101; 0 when there is no number.
		int productNumber(const std::string& product)
		{
			const std::string code = s100::sdb::productFromIdentifier(product);
			int number = 0;
			if (code.size() == 5)
			{
				(void)std::from_chars(code.data() + 2, code.data() + code.size(), number);
			}
			return number;
		}
	}

	SystemDatabaseChart::SystemDatabaseChart(LayerManager& parent, s100::sdb::SystemDatabase& database,
		const s100::sdb::ResourceStore& store, DatasetLayerSet::CacheLimit cache)
		: database_(&database), store_(&store),
		  drawer_(std::make_unique<LayerManager>(parent.GetScaler(), parent.catalogManager, parent.GetRenderResources()))
	{
		m_FileType = S100_FileType::FILE_ETC;
		SetFilePath(store.root().wstring());
		SetFileName(L"System Database");

		// Drawn one dataset after the other, smallest scale first, so a larger scale chart covers
		// what lies under it; and the view is never refitted to what happens to be open.
		drawer_->onIC = false;
		drawer_->m_isScreenFitEnabled = false;

		layers_ = std::make_unique<DatasetLayerSet>(*drawer_,
			[this](s100::sdb::RecordId dataset) { return OpenDataset(dataset); }, std::move(cache));
		selector_ = s100::sdb::DatasetSelector::fromDatabase(*database_);
	}

	SystemDatabaseChart::~SystemDatabaseChart()
	{
		layers_.reset();   // its layers live in drawer_
	}

	std::unique_ptr<Layer> SystemDatabaseChart::CreateLayer(LayerManager& parent, s100::sdb::SystemDatabase& database,
		const s100::sdb::ResourceStore& store, DatasetLayerSet::CacheLimit cache)
	{
		auto layer = std::make_unique<Layer>();
		auto chart = std::make_unique<SystemDatabaseChart>(parent, database, store, std::move(cache));
		chart->m_pLayer = layer.get();
		layer->m_spatialObject = chart.release();
		static_cast<SystemDatabaseChart*>(layer->m_spatialObject)->UpdateExtent();
		return layer;
	}

	void SystemDatabaseChart::UpdateExtent()
	{
		if (!m_pLayer)
		{
			return;
		}
		MBR extent;
		for (const auto& coverage : selector_.coverages())
		{
			MBR box(coverage.bounds.minX, coverage.bounds.minY, coverage.bounds.maxX, coverage.bounds.maxY);
			projection(box);
			extent.CalcMBR(box);
		}
		if (selector_.coverages().empty())
		{
			// Nothing installed yet: the same world view a layer manager without layers shows.
			extent = MBR(-170.0, -30.0, 170.0, 30.0);
			projection(extent);
		}
		m_pLayer->SetMBR(extent);
	}

	void SystemDatabaseChart::Draw(HDC& hDC, Scaler* scaler, double offset)
	{
		if (!scaler)
		{
			return;
		}
		const auto view = selectionView(*scaler);
		if (!view)
		{
			return;
		}
		const auto selection = selector_.select(*view);
		layers_->show(s100::sdb::DatasetSelector::datasetsOf(selection));
		drawer_->Draw(hDC, static_cast<int>(offset));
	}

	void SystemDatabaseChart::Reload()
	{
		layers_->clear();
		selector_ = s100::sdb::DatasetSelector::fromDatabase(*database_);
		UpdateExtent();
	}

	void SystemDatabaseChart::CataloguesChanged()
	{
		layers_->forgetFailures();
	}

	std::unique_ptr<Layer> SystemDatabaseChart::OpenDataset(s100::sdb::RecordId dataset)
	{
		const auto record = database_->datasets().find(dataset);
		if (!record || record->status != s100::sdb::DatasetStatus::Active || !drawer_->catalogManager)
		{
			return nullptr;
		}
		const auto catalogues = drawer_->catalogManager->resolve(record->product, record->productEdition);
		if (!catalogues.fc)
		{
			return nullptr;   // no feature catalogue for the product: kept until one is installed
		}
		return drawer_->CreateS100Layer(store_->absolute(record->path).wstring(), productNumber(record->product),
			catalogues.fc, catalogues.pc);
	}
}
