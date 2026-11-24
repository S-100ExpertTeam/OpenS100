#pragma once

#include "../SystemDatabase/LruCache.h"
#include "../SystemDatabase/Records.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
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
	//
	// With background opening on (DatasetOpening.h) and a planner given, show() does not open a
	// dataset itself: it asks the worker for it and goes on. The layers the worker has made are
	// taken the next time show() is called, all that are ready at once. Until every dataset of
	// the selection is there, the ones shown before stay, underneath, so the view is never
	// empty where it had a chart.
	class DatasetLayerSet final
	{
	public:
		using DatasetId = s100::sdb::RecordId;
		// Opens a dataset as a layer, not yet added anywhere; nullptr when it cannot be opened.
		using Opener = std::function<std::unique_ptr<Layer>(DatasetId)>;
		// The opening of one dataset, to be run on the worker thread.
		using Job = std::function<std::unique_ptr<Layer>()>;
		// Plans the opening of a dataset: called on the thread that draws, it reads what only
		// that thread may (the database, the catalogues to use) and returns the rest as a job.
		// An empty job: the dataset is opened where it is asked for, by the opener.
		using Planner = std::function<Job(DatasetId)>;
		// What a cached dataset costs against the cache capacity: 1 bounds the number of cells,
		// the file size bounds memory roughly.
		using Cost = std::function<std::size_t(DatasetId)>;

		struct CacheLimit
		{
			std::size_t capacity = 16;
			Cost cost;   // empty: one unit per dataset
		};

		DatasetLayerSet(LayerManager& drawer, Opener opener);
		DatasetLayerSet(LayerManager& drawer, Opener opener, CacheLimit cacheLimit);
		DatasetLayerSet(const DatasetLayerSet&) = delete;
		DatasetLayerSet& operator=(const DatasetLayerSet&) = delete;
		~DatasetLayerSet();

		// What makes the jobs of the worker; without one every dataset is opened by the opener.
		void setPlanner(Planner planner);

		// Makes the drawer hold the layers of exactly these datasets, given in selection order (the
		// larger scale first), and orders them to be drawn the other way round so the larger scale
		// lies on top. Datasets that leave go to the cache; datasets that come back come out of it;
		// the rest are opened. A dataset that fails to open is not tried again until
		// forgetFailures().
		void show(std::span<const DatasetId> datasets);

		// Datasets of the last selection the worker has not delivered yet.
		[[nodiscard]] std::size_t pendingCount() const;

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
		// The layer of a shown dataset; null when it is not shown.
		[[nodiscard]] const Layer* layerOf(DatasetId dataset) const
		{
			const auto found = shown_.find(dataset);
			return found == shown_.end() ? nullptr : found->second;
		}

	private:
		[[nodiscard]] std::size_t costOf(DatasetId dataset) const;
		// Takes what the worker has made since the last time; true if anything came.
		void adopt(std::span<const DatasetId> selected);
		// Asks the worker for a dataset, unless it is asked for already.
		void request(DatasetId dataset, Job job);
		// Forgets what was asked of the worker: what it delivers from now on is thrown away.
		void forgetRequests();

		// What this set and the tasks it gave the worker share.
		struct Requests
		{
			std::mutex mutex;
			std::uint64_t generation = 0;                               // tasks of an older one deliver nothing
			std::unordered_set<DatasetId> asked;                        // queued or being opened
			std::unordered_set<DatasetId> wanted;                       // of the selection in hand
			std::unordered_map<DatasetId, std::unique_ptr<Layer>> ready; // null: could not be opened
		};

		LayerManager* drawer_;
		Opener opener_;
		Planner planner_;
		std::shared_ptr<Requests> requests_;
		Cost cost_;
		std::unordered_map<DatasetId, Layer*> shown_;   // owned by the drawer's layer list
		s100::sdb::LruCache<DatasetId, std::unique_ptr<Layer>> cache_;
		std::unordered_set<DatasetId> failed_;
		std::size_t opened_ = 0;
	};
}
