#pragma once

#include "../SystemDatabase/LruCache.h"
#include "../SystemDatabase/Records.h"

#include <cstddef>
#include <functional>
#include <memory>
#include <span>
#include <unordered_map>
#include <unordered_set>

class Layer;
class LayerManager;

namespace GISLibrary
{
	// The layers of the datasets a selection asks for (S-98 E-1.3), kept in a LayerManager that
	// draws them. Datasets that leave the selection are not deleted but kept in an LRU cache, so a
	// view that comes back shows them again without parsing them anew
	// (Documents/TechDocs/2026-11-10-system-database.md, SDB-6).
	class DatasetLayerSet final
	{
	public:
		using DatasetId = s100::sdb::RecordId;
		// Opens a dataset as a layer, not yet added anywhere; nullptr when it cannot be opened.
		using Opener = std::function<std::unique_ptr<Layer>(DatasetId)>;
		// What a cached dataset costs against the cache capacity: 1 bounds the number of cells,
		// the file size bounds memory roughly.
		using Cost = std::function<std::size_t(DatasetId)>;

		struct CacheLimit
		{
			std::size_t capacity = 16;
			Cost cost;   // empty: one unit per dataset
		};

		DatasetLayerSet(LayerManager& drawer, Opener opener, CacheLimit cacheLimit = {});
		DatasetLayerSet(const DatasetLayerSet&) = delete;
		DatasetLayerSet& operator=(const DatasetLayerSet&) = delete;
		~DatasetLayerSet();

		// Makes the drawer hold the layers of exactly these datasets, given in selection order (the
		// larger scale first), and orders them to be drawn the other way round so the larger scale
		// lies on top. Datasets that leave go to the cache; datasets that come back come out of it;
		// the rest are opened. A dataset that fails to open is not tried again until
		// forgetFailures().
		void show(std::span<const DatasetId> datasets);

		// Drops every layer, shown and cached (the installed datasets changed).
		void clear();
		// Drops the cached layers only (portrayal settings changed; shown layers are rebuilt by
		// the drawer).
		void dropCache();
		// Lets datasets that failed to open be tried again (new catalogues were installed).
		void forgetFailures();

		[[nodiscard]] std::size_t shownCount() const noexcept { return shown_.size(); }
		[[nodiscard]] std::size_t cachedCount() const noexcept { return cache_.size(); }
		// Datasets opened from their files so far (not counting those taken from the cache).
		[[nodiscard]] std::size_t openCount() const noexcept { return opened_; }
		[[nodiscard]] bool isShown(DatasetId dataset) const { return shown_.contains(dataset); }
		[[nodiscard]] bool isCached(DatasetId dataset) const { return cache_.contains(dataset); }

	private:
		[[nodiscard]] std::size_t costOf(DatasetId dataset) const;

		LayerManager* drawer_;
		Opener opener_;
		Cost cost_;
		std::unordered_map<DatasetId, Layer*> shown_;   // owned by the drawer's layer list
		s100::sdb::LruCache<DatasetId, std::unique_ptr<Layer>> cache_;
		std::unordered_set<DatasetId> failed_;
		std::size_t opened_ = 0;
	};
}
