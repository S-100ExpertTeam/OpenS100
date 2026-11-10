#include "stdafx.h"
#include "DatasetLayerSet.h"

#include "Layer.h"
#include "LayerManager.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace GISLibrary
{
	DatasetLayerSet::DatasetLayerSet(LayerManager& drawer, Opener opener, CacheLimit cacheLimit)
		: drawer_(&drawer), opener_(std::move(opener)), cost_(std::move(cacheLimit.cost)), cache_(cacheLimit.capacity)
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

	void DatasetLayerSet::show(std::span<const DatasetId> datasets)
	{
		// Datasets no longer selected leave the drawer. They go into the cache only after the
		// returning datasets have been taken out of it, or they could push those out.
		std::vector<std::pair<DatasetId, std::unique_ptr<Layer>>> leaving;
		for (auto shown = shown_.begin(); shown != shown_.end();)
		{
			if (std::ranges::find(datasets, shown->first) != datasets.end())
			{
				++shown;
				continue;
			}
			if (auto layer = drawer_->ReleaseLayer(shown->second))
			{
				leaving.emplace_back(shown->first, std::move(layer));
			}
			shown = shown_.erase(shown);
		}

		// Selected datasets come back from the cache, or are opened.
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
		for (auto& [dataset, layer] : leaving)
		{
			cache_.put(dataset, std::move(layer), costOf(dataset));
		}

		// The dataset selected last (the smallest scale) is drawn first, at the bottom.
		std::unordered_map<const Layer*, std::size_t> selectionOrder;
		for (std::size_t i = 0; i < datasets.size(); ++i)
		{
			if (const auto shown = shown_.find(datasets[i]); shown != shown_.end())
			{
				selectionOrder.try_emplace(shown->second, i);
			}
		}
		drawer_->layers.sort([&selectionOrder](const Layer* a, const Layer* b) {
			const auto rank = [&](const Layer* layer) {
				const auto found = selectionOrder.find(layer);
				return found == selectionOrder.end() ? std::size_t{ 0 } : found->second;
			};
			return rank(a) > rank(b);
		});
	}

	void DatasetLayerSet::clear()
	{
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
		cache_.clear();
	}

	void DatasetLayerSet::forgetFailures()
	{
		failed_.clear();
	}
}
