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
#include <unordered_set>

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
		LoadSelection();
	}

	void SystemDatabaseChart::LoadSelection()
	{
		selector_ = s100::sdb::DatasetSelector::fromDatabase(*database_);

		std::unordered_set<s100::sdb::RecordId> scaled;
		for (const auto& coverage : selector_.coverages())
		{
			scaled.insert(coverage.dataset);
		}
		unscaled_.clear();
		for (const auto& dataset : database_->datasets().all())
		{
			if (dataset.status == s100::sdb::DatasetStatus::Active && dataset.bounds && !scaled.contains(dataset.id))
			{
				unscaled_.push_back({ dataset.id, *dataset.bounds, dataset.format != s100::sdb::DatasetFormat::Hdf5 });
			}
		}
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
		std::vector<s100::sdb::BoundingBox> bounds;
		for (const auto& coverage : selector_.coverages())
		{
			bounds.push_back(coverage.bounds);
		}
		for (const auto& dataset : unscaled_)
		{
			bounds.push_back(dataset.bounds);
		}
		for (const auto& bound : bounds)
		{
			MBR box(bound.minX, bound.minY, bound.maxX, bound.maxY);
			projection(box);
			extent.CalcMBR(box);
		}
		if (bounds.empty())
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
		const auto selected = s100::sdb::DatasetSelector::datasetsOf(selection);

		// Selection order is the larger scale first, and the first is drawn last, on top: the
		// vector overlays (warnings, catalogues of products) before the charts, the grids after.
		std::vector<s100::sdb::RecordId> datasets;
		std::vector<s100::sdb::RecordId> grids;
		for (const auto& dataset : unscaled_)
		{
			const bool inView = !view->ground
				|| (dataset.bounds.minX <= view->ground->maxX && dataset.bounds.maxX >= view->ground->minX
					&& dataset.bounds.minY <= view->ground->maxY && dataset.bounds.maxY >= view->ground->minY);
			if (inView)
			{
				(dataset.overlay ? datasets : grids).push_back(dataset.id);
			}
		}
		datasets.insert(datasets.end(), selected.begin(), selected.end());
		datasets.insert(datasets.end(), grids.begin(), grids.end());
		layers_->show(datasets);

		// The charts of one scale band are drawn as one: adjacent cells do not cover each
		// other's symbols where they spill over the common edge.
		drawer_->drawGroups.clear();
		for (const auto& coverage : selection)
		{
			if (const Layer* layer = layers_->layerOf(coverage.coverage->dataset))
			{
				drawer_->drawGroups.try_emplace(layer, coverage.scaleBand);
			}
		}
		drawer_->Draw(hDC, static_cast<int>(offset));
	}

	void SystemDatabaseChart::Reload()
	{
		layers_->clear();
		LoadSelection();
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
