#include "stdafx.h"
#include "DatasetLayerSet.h"

#include "DatasetOpening.h"
#include "Layer.h"
#include "LayerManager.h"

#include <algorithm>
#include <limits>
#include <utility>
#include <vector>

namespace
{
	// The place in the drawing order of a dataset kept only until the selection is complete.
	constexpr std::size_t datasetsKeptBelow = (std::numeric_limits<std::size_t>::max)();
}

namespace GISLibrary
{
	DatasetLayerSet::DatasetLayerSet(LayerManager& drawer, Opener opener)
		: DatasetLayerSet(drawer, std::move(opener), CacheLimit{})
	{
	}

	DatasetLayerSet::DatasetLayerSet(LayerManager& drawer, Opener opener, CacheLimit cacheLimit)
		: drawer_(&drawer), opener_(std::move(opener)), requests_(std::make_shared<Requests>()), cost_(std::move(cacheLimit.cost)),
		  cache_(cacheLimit.capacity)
	{
	}

	DatasetLayerSet::~DatasetLayerSet()
	{
		clear();
	}

	std::size_t DatasetLayerSet::costOf(DatasetId dataset) const
	{
		return cost_ ? cost_(dataset) : 1;
	}

	void DatasetLayerSet::setPlanner(Planner planner)
	{
		planner_ = std::move(planner);
	}

	std::size_t DatasetLayerSet::pendingCount() const
	{
		const std::lock_guard<std::mutex> lock(requests_->mutex);
		std::size_t count = 0;
		for (const DatasetId dataset : requests_->asked)
		{
			count += requests_->wanted.contains(dataset) ? 1 : 0;
		}
		return count;
	}

	void DatasetLayerSet::adopt(std::span<const DatasetId> selected)
	{
		std::unordered_map<DatasetId, std::unique_ptr<Layer>> ready;
		{
			const std::lock_guard<std::mutex> lock(requests_->mutex);
			ready.swap(requests_->ready);
		}
		for (auto& [dataset, layer] : ready)
		{
			if (!layer)
			{
				failed_.insert(dataset);
				continue;
			}
			++opened_;
			if (shown_.contains(dataset))
			{
				continue;   // opened here meanwhile: the one shown stays
			}
			if (std::ranges::find(selected, dataset) == selected.end())
			{
				// The view has moved on; a view that comes back finds it in the cache.
				cache_.put(dataset, std::move(layer), costOf(dataset));
				continue;
			}
			Layer* raw = layer.release();
			drawer_->AddLayer(raw);
			shown_.emplace(dataset, raw);
		}
	}

	void DatasetLayerSet::request(DatasetId dataset, Job job)
	{
		std::uint64_t generation = 0;
		{
			const std::lock_guard<std::mutex> lock(requests_->mutex);
			if (!requests_->asked.insert(dataset).second)
			{
				return;
			}
			generation = requests_->generation;
		}
		BackgroundOpening::Run([requests = requests_, dataset, generation, job = std::move(job)]() {
			{
				// Not opened if nobody wants it any more by the time its turn comes.
				const std::lock_guard<std::mutex> lock(requests->mutex);
				if (generation != requests->generation)
				{
					return;
				}
				if (!requests->wanted.contains(dataset))
				{
					requests->asked.erase(dataset);
					return;
				}
			}
			std::unique_ptr<Layer> layer;
			{
				const std::lock_guard<std::recursive_mutex> opening(OpeningLock());
				layer = job();
			}
			{
				const std::lock_guard<std::mutex> lock(requests->mutex);
				if (generation == requests->generation)
				{
					requests->asked.erase(dataset);
					requests->ready[dataset] = std::move(layer);
				}
			}
			BackgroundOpening::NotifyReady();
		});
	}

	void DatasetLayerSet::forgetRequests()
	{
		const std::lock_guard<std::mutex> lock(requests_->mutex);
		requests_->generation++;
		requests_->asked.clear();
		requests_->wanted.clear();
		requests_->ready.clear();
	}

	void DatasetLayerSet::show(std::span<const DatasetId> datasets)
	{
		const bool background = planner_ && BackgroundOpening::Enabled();
		{
			const std::lock_guard<std::mutex> lock(requests_->mutex);
			requests_->wanted.clear();
			requests_->wanted.insert(datasets.begin(), datasets.end());
		}
		// What the worker has made since the last time, all of it at once.
		adopt(datasets);

		// Selected datasets come back from the cache, or are opened: here, or by the worker.
		bool waiting = false;
		for (const DatasetId dataset : datasets)
		{
			if (shown_.contains(dataset) || failed_.contains(dataset))
			{
				continue;
			}
			std::unique_ptr<Layer> layer;
			if (auto cached = cache_.take(dataset))
			{
				layer = std::move(*cached);
			}
			else
			{
				if (background)
				{
					bool asked = false;
					{
						const std::lock_guard<std::mutex> lock(requests_->mutex);
						asked = requests_->asked.contains(dataset);
					}
					if (asked)
					{
						waiting = true;
						continue;
					}
					if (Job job = planner_(dataset))
					{
						request(dataset, std::move(job));
						waiting = true;
						continue;
					}
				}
				layer = opener_ ? opener_(dataset) : nullptr;
				++opened_;
				if (!layer)
				{
					failed_.insert(dataset);
					continue;
				}
			}
			Layer* raw = layer.release();
			drawer_->AddLayer(raw);
			shown_.emplace(dataset, raw);
		}

		// Datasets no longer selected leave the drawer for the cache, after the returning ones
		// have been taken out of it, or they could push those out. While the worker still owes
		// a dataset of the selection they stay where they are: the chart shown before is better
		// than a hole.
		if (!waiting)
		{
			for (auto shown = shown_.begin(); shown != shown_.end();)
			{
				if (std::ranges::find(datasets, shown->first) != datasets.end())
				{
					++shown;
					continue;
				}
				if (auto layer = drawer_->ReleaseLayer(shown->second))
				{
					cache_.put(shown->first, std::move(layer), costOf(shown->first));
				}
				shown = shown_.erase(shown);
			}
		}

		// The dataset selected last (the smallest scale) is drawn first, at the bottom; one that
		// is only kept until the selection is complete lies under them all.
		std::unordered_map<const Layer*, std::size_t> selectionOrder;
		for (std::size_t i = 0; i < datasets.size(); ++i)
		{
			if (const auto shown = shown_.find(datasets[i]); shown != shown_.end())
			{
				selectionOrder.try_emplace(shown->second, i);
			}
		}
		std::unordered_set<const Layer*> kept;
		for (const auto& [dataset, layer] : shown_)
		{
			if (!selectionOrder.contains(layer))
			{
				kept.insert(layer);
			}
		}
		drawer_->layers.sort([&selectionOrder, &kept](const Layer* a, const Layer* b) {
			const auto rank = [&](const Layer* layer) {
				if (kept.contains(layer))
				{
					return datasetsKeptBelow;
				}
				const auto found = selectionOrder.find(layer);
				return found == selectionOrder.end() ? std::size_t{ 0 } : found->second;
			};
			return rank(a) > rank(b);
		});
	}

	void DatasetLayerSet::clear()
	{
		// The worker may be opening a dataset for this set, through the drawer: nothing of the set
		// goes while it is.
		forgetRequests();
		BackgroundOpening::WaitUntilIdle();
		forgetRequests();

		for (const auto& [dataset, layer] : shown_)
		{
			(void)drawer_->ReleaseLayer(layer);   // deleted here
		}
		shown_.clear();
		cache_.clear();
		failed_.clear();
	}

	void DatasetLayerSet::dropCache()
	{
		// What the worker is opening was portrayed with the settings before: asked for again.
		forgetRequests();
		cache_.clear();
	}

	void DatasetLayerSet::forgetFailures()
	{
		failed_.clear();
	}
}
