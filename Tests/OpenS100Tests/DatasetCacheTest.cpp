#include <gtest/gtest.h>
#include <windows.h>
#include "DatasetLayerSet.h"
#include "Layer.h"
#include "LayerManager.h"
#include "SystemDatabase/LruCache.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

// System Database work item SDB-6: datasets that leave the selection stay open in an LRU cache.
using s100::sdb::LruCache;

TEST(LruCache, EvictsTheLeastRecentlyUsed)
{
	LruCache<int, std::string> cache(2);
	cache.put(1, "one");
	cache.put(2, "two");
	ASSERT_NE(cache.find(1), nullptr);   // 1 is now the most recent
	cache.put(3, "three");

	EXPECT_TRUE(cache.contains(1));
	EXPECT_FALSE(cache.contains(2));
	EXPECT_TRUE(cache.contains(3));
	EXPECT_EQ(cache.size(), 2u);
}

TEST(LruCache, BoundsTheTotalCostAndDropsWhatCannotFit)
{
	LruCache<int, std::string> cache(100);
	cache.put(1, "a", 60);
	cache.put(2, "b", 30);
	EXPECT_EQ(cache.cost(), 90u);
	cache.put(3, "c", 30);   // 120 > 100: 1 goes
	EXPECT_FALSE(cache.contains(1));
	EXPECT_EQ(cache.cost(), 60u);

	cache.put(4, "too big", 150);
	EXPECT_FALSE(cache.contains(4));
	EXPECT_EQ(cache.cost(), 0u);
}

TEST(LruCache, TakeHandsTheValueBackAndReplacingKeepsOneEntry)
{
	LruCache<int, std::unique_ptr<int>> cache(4);
	cache.put(7, std::make_unique<int>(70));
	cache.put(7, std::make_unique<int>(71));
	EXPECT_EQ(cache.size(), 1u);

	auto taken = cache.take(7);
	ASSERT_TRUE(taken);
	EXPECT_EQ(**taken, 71);
	EXPECT_TRUE(cache.empty());
	EXPECT_FALSE(cache.take(7));

	cache.put(1, std::make_unique<int>(1));
	cache.put(2, std::make_unique<int>(2));
	cache.setCapacity(1);
	EXPECT_EQ(cache.size(), 1u);
	EXPECT_TRUE(cache.contains(2));
}

// --- DatasetLayerSet over a LayerManager ----------------------------------------------------------

namespace
{
	// Counts the layers alive, to see that nothing leaks or is deleted twice.
	int liveLayers = 0;

	class CountedLayer final : public Layer
	{
	public:
		explicit CountedLayer(int dataset) : dataset(dataset) { ++liveLayers; }
		~CountedLayer() override { --liveLayers; }
		int dataset;
	};

	class DatasetLayerSetTest : public ::testing::Test
	{
	protected:
		void SetUp() override
		{
			liveLayers = 0;
			drawer.m_isScreenFitEnabled = false;
		}

		std::vector<int> drawOrder() const
		{
			std::vector<int> order;
			for (const Layer* layer : drawer.layers)
			{
				order.push_back(static_cast<const CountedLayer*>(layer)->dataset);
			}
			return order;
		}

		LayerManager drawer{ nullptr };
		std::map<GISLibrary::DatasetLayerSet::DatasetId, int> opens;
		GISLibrary::DatasetLayerSet::Opener opener = [this](GISLibrary::DatasetLayerSet::DatasetId dataset) -> std::unique_ptr<Layer> {
			++opens[dataset];
			if (dataset == 99)
			{
				return nullptr;   // a dataset that cannot be opened
			}
			return std::make_unique<CountedLayer>(static_cast<int>(dataset));
		};
	};
}

TEST_F(DatasetLayerSetTest, ShowsTheSelectionDrawingTheLargerScaleOnTop)
{
	GISLibrary::DatasetLayerSet set(drawer, opener);
	const std::vector<GISLibrary::DatasetLayerSet::DatasetId> selection = { 1, 2, 3 };   // larger scale first

	set.show(selection);

	EXPECT_EQ(drawOrder(), (std::vector<int>{ 3, 2, 1 }));
	EXPECT_EQ(set.shownCount(), 3u);
	EXPECT_EQ(set.openCount(), 3u);
}

TEST_F(DatasetLayerSetTest, KeepsDeselectedDatasetsOpenAndReusesThem)
{
	GISLibrary::DatasetLayerSet set(drawer, opener, { .capacity = 2 });
	set.show(std::vector<GISLibrary::DatasetLayerSet::DatasetId>{ 1, 2 });
	set.show(std::vector<GISLibrary::DatasetLayerSet::DatasetId>{ 3 });

	EXPECT_EQ(drawOrder(), (std::vector<int>{ 3 }));
	EXPECT_TRUE(set.isCached(1));
	EXPECT_TRUE(set.isCached(2));
	EXPECT_EQ(liveLayers, 3);

	// Back to the first view: nothing is parsed again.
	set.show(std::vector<GISLibrary::DatasetLayerSet::DatasetId>{ 2, 1 });
	EXPECT_EQ(opens[1], 1);
	EXPECT_EQ(opens[2], 1);
	EXPECT_EQ(drawOrder(), (std::vector<int>{ 1, 2 }));
	EXPECT_TRUE(set.isCached(3));

	// Beyond the capacity the least recently used layers are deleted.
	set.show(std::vector<GISLibrary::DatasetLayerSet::DatasetId>{ 4, 5, 6 });
	EXPECT_EQ(set.cachedCount(), 2u);
	EXPECT_EQ(liveLayers, 5);   // 4, 5, 6 shown and two of 1, 2, 3 cached
}

TEST_F(DatasetLayerSetTest, DoesNotRetryADatasetThatFailedToOpenUntilAsked)
{
	GISLibrary::DatasetLayerSet set(drawer, opener);
	const std::vector<GISLibrary::DatasetLayerSet::DatasetId> selection = { 99, 1 };
	set.show(selection);
	set.show(selection);
	EXPECT_EQ(opens[99], 1);
	EXPECT_EQ(set.shownCount(), 1u);

	set.forgetFailures();
	set.show(selection);
	EXPECT_EQ(opens[99], 2);
}

TEST_F(DatasetLayerSetTest, ClearingAndDestroyingDeleteEveryLayer)
{
	{
		GISLibrary::DatasetLayerSet set(drawer, opener);
		set.show(std::vector<GISLibrary::DatasetLayerSet::DatasetId>{ 1, 2 });
		set.show(std::vector<GISLibrary::DatasetLayerSet::DatasetId>{ 3 });
		set.clear();
		EXPECT_EQ(liveLayers, 0);
		EXPECT_TRUE(drawer.layers.empty());

		set.show(std::vector<GISLibrary::DatasetLayerSet::DatasetId>{ 4 });
	}
	EXPECT_EQ(liveLayers, 0);
	EXPECT_TRUE(drawer.layers.empty());
}
