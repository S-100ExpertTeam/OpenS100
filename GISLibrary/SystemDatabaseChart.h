#pragma once

#include "DatasetLayerSet.h"
#include "LayerContainer.h"
#include "SpatialObject.h"

#include "../SystemDatabase/DatasetSelection.h"

#include <memory>
#include <vector>

class Layer;
class LayerManager;

namespace s100::sdb
{
	class ResourceStore;
	class SystemDatabase;
}

namespace GISLibrary
{
	// The datasets installed in the System Database, drawn as one layer. Nothing is opened up
	// front: the coverages are read from the database, and each frame S-98 E-1.3 picks the
	// datasets the view needs; those are opened (with the catalogues their product edition
	// resolves to) and the ones that leave the view are kept in a cache for a while
	// (Documents/TechDocs/2026-11-10-system-database.md, SDB-7). Datasets without display scales
	// take no part in E-1.3; they are shown whenever their bounding box meets the view: the gridded
	// products (S-102, S-104, S-111, ...) under the selected ones, the vector ones (S-124, S-128,
	// ...) over them. The selected datasets of one scale band are drawn as one chart
	// (LayerManager::drawGroups).
	class SystemDatabaseChart final : public SpatialObject, public LayerContainer
	{
	public:
		// `parent` is the layer manager the chart's layer goes into; its scaler, catalogue
		// manager and renderer are used for the datasets. The database and store must outlive
		// the chart.
		SystemDatabaseChart(LayerManager& parent, s100::sdb::SystemDatabase& database, const s100::sdb::ResourceStore& store,
			DatasetLayerSet::CacheLimit cache = {});
		~SystemDatabaseChart() override;

		// The layer that shows the chart, for LayerManager::AddLayer. Its extent is that of the
		// installed datasets.
		[[nodiscard]] static std::unique_ptr<Layer> CreateLayer(LayerManager& parent, s100::sdb::SystemDatabase& database,
			const s100::sdb::ResourceStore& store, DatasetLayerSet::CacheLimit cache = {});

		bool Open(const std::wstring& /*path*/) override { return true; }
		bool OpenMetadata(const std::wstring& /*path*/) override { return true; }
		void Draw(HDC& hDC, Scaler* scaler, double offset = 0) override;

		// Reads the coverages again and closes every open dataset: after datasets were installed
		// or removed.
		void Reload();
		// Lets datasets that could not be opened for want of a catalogue be tried again.
		void CataloguesChanged();

		LayerManager* GetInnerLayers() override { return drawer_.get(); }
		void DropCachedLayers() override { layers_->dropCache(); }

		[[nodiscard]] const DatasetLayerSet& GetLayerSet() const noexcept { return *layers_; }
		[[nodiscard]] const s100::sdb::DatasetSelector& GetSelector() const noexcept { return selector_; }

	private:
		struct UnscaledDataset
		{
			s100::sdb::RecordId id = 0;
			s100::sdb::BoundingBox bounds;   // longitude/latitude
			bool overlay = false;            // drawn over the selected charts: vector products, S-111 arrows
		};

		[[nodiscard]] std::unique_ptr<Layer> OpenDataset(s100::sdb::RecordId dataset);
		void UpdateExtent();
		void LoadSelection();

		s100::sdb::SystemDatabase* database_;
		const s100::sdb::ResourceStore* store_;
		std::unique_ptr<LayerManager> drawer_;
		s100::sdb::DatasetSelector selector_;
		std::vector<UnscaledDataset> unscaled_;
		std::unique_ptr<DatasetLayerSet> layers_;
	};
}
