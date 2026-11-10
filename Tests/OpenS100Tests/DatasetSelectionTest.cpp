#include <gtest/gtest.h>
#include "SystemDatabase/DatasetSelection.h"
#include "SystemDatabase/SystemDatabase.h"

#include <initializer_list>
#include <string>

// System Database work item SDB-5: S-98 Appendix E-1 scale bands and Data Coverage selection.
// The cases and expected results are those of opens100-web packages/core/test/dataset-loading.test.ts,
// so the two implementations are held to the same answers.
using namespace s100::sdb;

namespace
{
	Ring rectangle(double x0, double y0, double x1, double y1)
	{
		return { { x0, y0 }, { x1, y0 }, { x1, y1 }, { x0, y1 }, { x0, y0 } };
	}

	std::vector<DevicePoint> deviceRing(const Ring& ring)
	{
		std::vector<DevicePoint> points;
		for (const Position& p : ring)
		{
			points.push_back({ p.lon, p.lat });
		}
		return points;
	}

	std::vector<int> bandList(const ScaleBands& bands)
	{
		std::vector<int> list;
		for (int band = 1; band <= 15; ++band)
		{
			if (bands.test(static_cast<std::size_t>(band)))
			{
				list.push_back(band);
			}
		}
		return list;
	}

	SelectableCoverage coverage(RecordId dataset, std::initializer_list<int> bands, Ring ring)
	{
		SelectableCoverage item{ .dataset = dataset, .optimumDisplayScale = 1, .rings = { std::move(ring) } };
		for (const int band : bands)
		{
			item.bands.set(static_cast<std::size_t>(band));
		}
		item.bounds = *boundsOf(item.rings);
		return item;
	}

	const DeviceProjection identity = [](const Position& p) { return DevicePoint{ p.lon, p.lat }; };

	// "name@band" with '*' for overscaled, as the web tests print them.
	std::vector<std::string> describe(const std::vector<SelectedCoverage>& selection, const std::vector<std::string>& names)
	{
		std::vector<std::string> text;
		for (const auto& selected : selection)
		{
			text.push_back(names[static_cast<std::size_t>(selected.coverage->dataset)] + "@" + std::to_string(selected.scaleBand)
				+ (selected.overscaled ? "*" : ""));
		}
		return text;
	}

	using Strings = std::vector<std::string>;
}

TEST(ScaleBands, BandsADisplayScaleByTable11)
{
	EXPECT_EQ(scaleBandOf(20'000'000), 1);
	EXPECT_EQ(scaleBandOf(10'000'000), 1);
	EXPECT_EQ(scaleBandOf(9'999'999), 2);
	EXPECT_EQ(scaleBandOf(3'500'000), 2);
	EXPECT_EQ(scaleBandOf(22'000), 9);
	EXPECT_EQ(scaleBandOf(21'999), 10);
	EXPECT_EQ(scaleBandOf(1'000), 15);
	EXPECT_EQ(scaleBandOf(500), 15);
}

TEST(ScaleBands, SpansTheBandsBetweenMinimumAndOptimumExclusiveOfTouchingBands)
{
	EXPECT_EQ(bandList(scaleBandsOf(45'000, 12'000)), (std::vector<int>{ 9, 10 }));
	EXPECT_EQ(bandList(scaleBandsOf(350'000, 8'000)), (std::vector<int>{ 6, 7, 8, 9, 10, 11 }));
	EXPECT_EQ(bandList(scaleBandsOf(std::nullopt, 3'500'000)), (std::vector<int>{ 1, 2 }));
}

TEST(DeviceRegion, SubtractsPolygonsOnceReportingWhetherTheyOverlappedWhatWasLeft)
{
	DeviceRegion region(100, 100, 10);
	const std::vector<std::vector<DevicePoint>> half = { deviceRing(rectangle(0, 0, 50, 100)) };
	EXPECT_TRUE(region.subtract(half));
	EXPECT_EQ(region.size(), 50u);
	EXPECT_FALSE(region.subtract(half));
}

TEST(DeviceRegion, KeepsInteriorRingsAndIgnoresWhatLiesOffTheDevice)
{
	DeviceRegion region(100, 100, 10);
	const auto ring = [](double a, double b) { return deviceRing(rectangle(a, a, b, b)); };
	using Rings = std::vector<std::vector<DevicePoint>>;
	EXPECT_FALSE(region.subtract(Rings{ ring(-50, -10) }));
	(void)region.subtract(Rings{ ring(0, 100), ring(20, 80) });
	EXPECT_EQ(region.size(), 36u);
	EXPECT_FALSE(region.empty());
}

class SelectDataCoverages : public ::testing::Test
{
protected:
	const Strings names = { "detailed", "overview", "elsewhere" };
	const std::vector<SelectableCoverage> collection = {
		coverage(0, { 9, 10 }, rectangle(0, 0, 50, 100)),
		coverage(1, { 6, 7, 8 }, rectangle(0, 0, 100, 100)),
		coverage(2, { 9, 10 }, rectangle(200, 0, 300, 100)),
	};
	const SelectableCoverage& detailed = collection[0];
	const SelectableCoverage& overview = collection[1];
	const SelectableCoverage& elsewhere = collection[2];

	Strings select(double scale, std::span<const SelectableCoverage* const> pinned = {}) const
	{
		DeviceRegion viewport(100, 100, 10);
		return describe(selectDataCoverages(collection, scale, viewport, identity, std::nullopt, pinned), names);
	}
};

TEST_F(SelectDataCoverages, FillsWhatTheDisplayBandLeavesUncoveredFromSmallerScaleBandsAsOverscaled)
{
	EXPECT_EQ(select(20'000), (Strings{ "detailed@10", "overview@8*" }));
}

TEST_F(SelectDataCoverages, StopsOnceTheViewportIsCovered)
{
	EXPECT_EQ(select(80'000), (Strings{ "overview@8" }));
}

TEST_F(SelectDataCoverages, LeavesACoverageOutsideTheGroundTheViewportCanShowUnprojected)
{
	int projected = 0;
	const DeviceProjection counting = [&](const Position& p) { ++projected; return DevicePoint{ p.lon, p.lat }; };
	const std::vector<SelectableCoverage> two = { detailed, elsewhere };
	const BoundingBox within{ -1, -1, 120, 120 };

	DeviceRegion viewport(100, 100, 10);
	const auto selected = selectDataCoverages(two, 20'000, viewport, counting, within);
	ASSERT_EQ(selected.size(), 1u);
	EXPECT_EQ(selected[0].coverage->dataset, 0);
	EXPECT_EQ(projected, static_cast<int>(detailed.rings[0].size()));

	// Without the ground bounds every coverage of the band is projected, off the device or not.
	projected = 0;
	DeviceRegion again(100, 100, 10);
	(void)selectDataCoverages(two, 20'000, again, counting);
	EXPECT_EQ(projected, static_cast<int>(detailed.rings[0].size() + elsewhere.rings[0].size()));
}

TEST_F(SelectDataCoverages, SelectsNothingWhenNoBandDownToOneHasCoverage)
{
	EXPECT_TRUE(select(1'000'000).empty());
}

TEST_F(SelectDataCoverages, TakesAPinnedCoverageFirstAtAnyScaleAndFillsTheRestAroundIt)
{
	const SelectableCoverage* pinDetailed[] = { &detailed };
	const SelectableCoverage* pinOverview[] = { &overview };
	const SelectableCoverage* pinElsewhere[] = { &elsewhere };
	// Zoomed out past its bands the detailed cell stays, in its smallest band, not overscaled.
	EXPECT_EQ(select(1'000'000, pinDetailed), (Strings{ "detailed@9" }));
	EXPECT_EQ(select(80'000, pinDetailed), (Strings{ "detailed@9", "overview@8" }));
	// Zoomed in past its bands it is overscaled; covering the whole viewport, it leaves no water
	// for the detailed cell.
	EXPECT_EQ(select(20'000, pinOverview), (Strings{ "overview@8*" }));
	// Within its bands it is selected as usual, and only once.
	EXPECT_EQ(select(20'000, pinDetailed), (Strings{ "detailed@10", "overview@8*" }));
	// Off the viewport it is still selected, and still selected first.
	EXPECT_EQ(select(20'000, pinElsewhere), (Strings{ "elsewhere@10", "detailed@10", "overview@8*" }));
}

// --- the selector over the database -----------------------------------------------------------

TEST(DatasetSelector, ReadsActiveCoveragesAndListsEachDatasetOnce)
{
	auto database = SystemDatabase::openInMemory();
	const auto add = [&](std::string name, int optimum, std::optional<int> minimum, const PolygonRings& rings) {
		const RecordId id = database.datasets().add(DatasetRecord{
			.name = name, .product = "S-101", .path = "DATASET/S-101/" + name + "/" + name + ".000", .importedAt = "now" });
		const DataCoverageRecord record{ .optimumDisplayScale = optimum, .minimumDisplayScale = minimum,
			.bounds = *boundsOf(rings), .polygon = encodeWkbPolygon(rings) };
		(void)database.coverages().replace(id, std::span(&record, 1));
		return id;
	};
	// An approach cell over the west half, with two coverage patches; a coastal cell under all of it.
	const RecordId approach = add("approach", 22'000, 90'000, { rectangle(0, 0, 25, 100) });
	const DataCoverageRecord second{ .optimumDisplayScale = 22'000, .minimumDisplayScale = 90'000,
		.bounds = { 25, 0, 50, 100 }, .polygon = encodeWkbPolygon(PolygonRings{ rectangle(25, 0, 50, 100) }) };
	{
		auto both = database.coverages().forDataset(approach);
		both.push_back(second);
		(void)database.coverages().replace(approach, both);
	}
	const RecordId coastal = add("coastal", 90'000, 350'000, { rectangle(0, 0, 100, 100) });
	(void)add("no scale", 0, std::nullopt, { rectangle(0, 0, 100, 100) });   // cannot take part

	const auto selector = DatasetSelector::fromDatabase(database);
	EXPECT_EQ(selector.coverages().size(), 3u);

	const DatasetSelector::View view{ .scale = 30'000, .width = 100, .height = 100, .project = identity, .cellSize = 10 };
	const auto selection = selector.select(view);
	ASSERT_EQ(selection.size(), 3u);   // both approach patches, then the coastal cell for the rest
	EXPECT_EQ(DatasetSelector::datasetsOf(selection), (std::vector<RecordId>{ approach, coastal }));
	EXPECT_FALSE(selection[0].overscaled);
	EXPECT_TRUE(selection[2].overscaled);

	EXPECT_TRUE(selector.select(DatasetSelector::View{ .scale = 30'000 }).empty());   // no device, no selection
}
