#pragma once

#include "SdbReaders.h"
#include "SystemDatabaseChart.h"

#include "../SystemDatabase/Importer.h"
#include "../SystemDatabase/ResourceStore.h"
#include "../SystemDatabase/SystemDatabase.h"

#include <filesystem>
#include <memory>

class CatalogManager;
class Layer;
class LayerManager;

namespace GISLibrary
{
	// A System Database opened for a viewer: the database and its managed store, an importer with
	// the readers this library has, and the chart layer that draws the installed datasets. Keeps
	// the catalogue manager and the chart in step with every import and removal made through it.
	class SystemDatabaseSession final
	{
	public:
		// Opens the database under `root` (creating both when missing). Throws
		// s100::sdb::DatabaseError when the database cannot be opened.
		explicit SystemDatabaseSession(std::filesystem::path root);
		SystemDatabaseSession(const SystemDatabaseSession&) = delete;
		SystemDatabaseSession& operator=(const SystemDatabaseSession&) = delete;
		~SystemDatabaseSession();

		// %LOCALAPPDATA%\OpenS100\SDB on Windows, $XDG_DATA_HOME/opens100/sdb (or
		// ~/.local/share/opens100/sdb) elsewhere: the default the viewer uses. The environment
		// variable OPENS100_SDB_ROOT overrides it (another store for testing or a shared one).
		[[nodiscard]] static std::filesystem::path defaultRoot();

		[[nodiscard]] s100::sdb::SystemDatabase& database() noexcept { return database_; }
		[[nodiscard]] const s100::sdb::ResourceStore& store() const noexcept { return store_; }
		[[nodiscard]] s100::sdb::Importer& importer() noexcept { return importer_; }

		// Registers the installed catalogues with `catalogues` (reading none) and adds the chart
		// layer to `layers`. The session must outlive both, or detach() first.
		void attach(LayerManager& layers, CatalogManager& catalogues, DatasetLayerSet::CacheLimit cache = {});
		// Removes the chart layer.
		void detach();
		// The chart, or null when it is not attached or its layer was deleted from the layer
		// manager.
		[[nodiscard]] SystemDatabaseChart* chart() const;

		// After catalogues were installed or removed through importer(): registers them again,
		// and lets the chart try the datasets it could not portray before.
		void refreshCatalogues();
		// After datasets were installed or removed through importer(): reloads the chart, which
		// closes every open dataset (and adds it again if its layer was deleted).
		void refreshDatasets();

	private:
		void addChart();

		s100::sdb::ResourceStore store_;
		s100::sdb::SystemDatabase database_;
		S101DatasetInspector datasetInspector_;
		Hdf5DatasetInspector gridInspector_;
		GmlDatasetInspector gmlInspector_;
		XmlCatalogueInspector catalogueInspector_;
		ExchangeCatalogueReader exchangeSetReader_;
		s100::sdb::Importer importer_;

		LayerManager* layers_ = nullptr;
		DatasetLayerSet::CacheLimit cache_;
		CatalogManager* catalogues_ = nullptr;
		SystemDatabaseChart* chart_ = nullptr;   // owned by chartLayer_
		Layer* chartLayer_ = nullptr;            // owned by layers_ while it lists it
	};
}
