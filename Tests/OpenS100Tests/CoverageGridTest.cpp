#include <gtest/gtest.h>

#include "Coverage/CoverageLookup.h"
#include "Coverage/CoverageModel.h"
#include "Coverage/CoverageTime.h"
#include "Coverage/RegularGrid.h"
#include "Coverage/Utm.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

// Coverage backport CV-1 (Documents/TechDocs/2026-11-11-coverage-backport-plan.md): the grid
// model, UTM, times and lookups, with the cases and expected values of opens100-web
// `packages/core/test/coverage.test.ts` and `utm.test.ts`.
using namespace s100::coverage;
using namespace std::chrono;

namespace
{
	const RegularGrid smallGrid{ .originLon = 128, .originLat = 34, .spacingLon = 0.5, .spacingLat = 0.25, .cols = 3, .rows = 2 };

	TimePoint hoursAfterEpoch(double h)
	{
		return TimePoint{ milliseconds(static_cast<long long>(h * 3600000)) };
	}

	std::vector<CoverageTimeGroup> groupsAt(std::initializer_list<double> hoursList)
	{
		std::vector<CoverageTimeGroup> groups;
		int i = 0;
		for (const double h : hoursList)
		{
			groups.push_back({ "Group_00" + std::to_string(++i), hoursAfterEpoch(h) });
		}
		return groups;
	}

	// The layout of NOAA's 102US004AK5FL262267: 16 m cells on zone 8N.
	const RegularGrid noaaGrid{ .originLon = 576958.7877, .originLat = 6050944.4675, .spacingLon = 16, .spacingLat = 16,
		.cols = 2458, .rows = 2138, .utm = UtmZone{ 8, false } };
}

TEST(CoverageTime, ReadsCompactAndExtendedFormsAsUtc)
{
	const TimePoint expected = sys_days{ 2026y / September / 14 } + 10h;
	EXPECT_EQ(parseS100DateTime("20260914T100000Z"), expected);
	EXPECT_EQ(parseS100DateTime("2026-09-14T10:00:00"), expected);
	EXPECT_EQ(parseS100DateTime("2026-09-14T19:00:00+09:00"), expected);
	EXPECT_EQ(parseS100DateTime(" 2026-09-14 10:00 "), expected);
}

TEST(CoverageTime, KeepsSmallYearsAndRejectsOtherText)
{
	const auto placeholder = parseS100DateTime("00010101T000000Z");
	ASSERT_TRUE(placeholder);
	EXPECT_EQ(year_month_day{ floor<days>(*placeholder) }.year(), year{ 1 });
	EXPECT_FALSE(parseS100DateTime("20260914"));
	EXPECT_FALSE(parseS100DateTime(""));
}

TEST(CoverageTime, TimeIndexIsTheLastGroupStartingAtOrBefore)
{
	const auto groups = groupsAt({ 0, 3, 6 });
	EXPECT_EQ(timeIndexAt(groups, hoursAfterEpoch(-1)), 0u);
	EXPECT_EQ(timeIndexAt(groups, hoursAfterEpoch(3)), 1u);
	EXPECT_EQ(timeIndexAt(groups, hoursAfterEpoch(5)), 1u);
	EXPECT_EQ(timeIndexAt(groups, hoursAfterEpoch(100)), 2u);
	EXPECT_EQ(timeIndexAt(groups, std::nullopt), 0u);
}

TEST(CoverageTime, DisplayTimeIndexFollowsS111Clause94)
{
	const auto groups = groupsAt({ 0, 1, 2 });
	EXPECT_EQ(displayTimeIndex(groups, TimePoint{ milliseconds(-1) }), std::nullopt);
	EXPECT_EQ(displayTimeIndex(groups, hoursAfterEpoch(0)), 0u);
	EXPECT_EQ(displayTimeIndex(groups, hoursAfterEpoch(1.5)), 1u);
	EXPECT_EQ(displayTimeIndex(groups, hoursAfterEpoch(2)), 2u);
	EXPECT_EQ(displayTimeIndex(groups, hoursAfterEpoch(2.5)), 2u);
	EXPECT_EQ(displayTimeIndex(groups, hoursAfterEpoch(3)), std::nullopt);

	const std::vector<CoverageTimeGroup> single{ { "Group_001", TimePoint{} } };
	EXPECT_EQ(displayTimeIndex(single, TimePoint{ minutes(2) }), 0u);
	EXPECT_EQ(displayTimeIndex(single, TimePoint{ minutes(10) }), std::nullopt);
	const std::vector<CoverageTimeGroup> staticGroup{ { "Group_001", std::nullopt } };
	EXPECT_EQ(displayTimeIndex(staticGroup, TimePoint{ milliseconds(123) }), 0u);
}

TEST(CoverageGrid, SamplesTheNearestNodeWithRowZeroAtTheOrigin)
{
	const std::vector<float> values{ 1, 2, 3, 4, 5, -999 };
	EXPECT_EQ(sampleGrid(smallGrid, values, -999, { 128.5, 34 }), 2.0f);
	EXPECT_EQ(sampleGrid(smallGrid, values, -999, { 128.3, 34.2 }), 5.0f);
	EXPECT_EQ(sampleGrid(smallGrid, values, -999, { 127.8, 34 }), 1.0f);
	EXPECT_EQ(sampleGrid(smallGrid, values, -999, { 127.7, 34 }), std::nullopt);
	EXPECT_EQ(sampleGrid(smallGrid, values, -999, { 129, 34.25 }), std::nullopt);   // a fill value
}

TEST(CoverageGrid, RangesAndBoundsIgnoreFillValues)
{
	const std::vector<float> values{ 1, 2, 3, 4, 5, -999 };
	EXPECT_EQ(valueRange(values, -999), (ValueRange{ 1, 5 }));
	const std::vector<float> nothing{ -999, std::numeric_limits<float>::quiet_NaN() };
	EXPECT_EQ(valueRange(nothing, -999), std::nullopt);
	EXPECT_EQ(gridBounds(smallGrid), (GeoBounds{ 128, 34, 129, 34.25 }));
}

TEST(CoverageGrid, FillValuesCompareAsFloat32WidenedToDouble)
{
	// 1000000 is exact in float32; -9999.9 is not, and only its float32 value matches a value read as float32.
	EXPECT_TRUE(isNoData(1000000.0f, 1000000.0));
	EXPECT_TRUE(isNoData(std::numeric_limits<float>::infinity(), 0));
	EXPECT_FALSE(isNoData(-9999.9f, -9999.9));
	EXPECT_TRUE(isNoData(-9999.9f, static_cast<double>(-9999.9f)));
}

TEST(CoverageLookup, HonoursEachIntervalClosureAndTakesTheFirstMatch)
{
	const auto entry = [](std::optional<double> lower, std::optional<double> upper, IntervalClosure closure) {
		return LookupEntry{ .label = {}, .lower = lower, .upper = upper, .closure = closure };
	};
	const std::vector<LookupEntry> lookup{ entry(std::nullopt, 0, IntervalClosure::LtSemiInterval),
		entry(0, 30, IntervalClosure::GeLtInterval), entry(30, std::nullopt, IntervalClosure::GeSemiInterval) };
	EXPECT_EQ(lookupCoverageEntry(lookup, -0.1)->index, 0u);
	EXPECT_EQ(lookupCoverageEntry(lookup, 0)->index, 1u);
	EXPECT_EQ(lookupCoverageEntry(lookup, 15), (LookupMatch{ 1, 0.5 }));
	EXPECT_EQ(lookupCoverageEntry(lookup, 30)->index, 2u);
	const auto one = [&](std::optional<double> lower, std::optional<double> upper, IntervalClosure closure, double value) {
		const std::vector<LookupEntry> single{ entry(lower, upper, closure) };
		return lookupCoverageEntry(single, value);
	};
	EXPECT_EQ(one(0, 10, IntervalClosure::OpenInterval, 0), std::nullopt);
	EXPECT_EQ(one(0, 10, IntervalClosure::GtLeInterval, 10)->index, 0u);
	EXPECT_EQ(one(0, 10, IntervalClosure::ClosedInterval, 10)->index, 0u);
	EXPECT_EQ(one(5, std::nullopt, IntervalClosure::GtSemiInterval, 5), std::nullopt);
	EXPECT_EQ(one(std::nullopt, 5, IntervalClosure::LeSemiInterval, 5)->index, 0u);
	EXPECT_EQ(intervalClosureFromString("geLtInterval"), IntervalClosure::GeLtInterval);
	EXPECT_EQ(intervalClosureFromString("half"), std::nullopt);
}

TEST(CoverageModel, ProductIdFromSpecification)
{
	EXPECT_EQ(productIdFromSpecification("INT.IHO.S-413.1.0"), "S-413");
	EXPECT_EQ(productIdFromSpecification("INT.IHO.S-102.3.0.0"), "S-102");
	EXPECT_EQ(productIdFromSpecification("unknown"), std::nullopt);
}

TEST(CoverageUtm, RecognisesWgs84UtmCodesOnly)
{
	EXPECT_EQ(utmZoneFromEpsg(32608), (UtmZone{ 8, false }));
	EXPECT_EQ(utmZoneFromEpsg(32752), (UtmZone{ 52, true }));
	EXPECT_EQ(utmZoneFromEpsg(4326), std::nullopt);
	EXPECT_EQ(utmZoneFromEpsg(32661), std::nullopt);
}

TEST(CoverageUtm, MatchesProjAndRoundTrips)
{
	// PROJ: echo "54.75 -133.5" | cs2cs EPSG:4326 EPSG:32608
	const UtmPoint point = geographicToUtm({ 8, false }, { -133.5, 54.75 });
	EXPECT_NEAR(point.easting, 596544.802, 0.005);
	EXPECT_NEAR(point.northing, 6068004.391, 0.005);

	const struct { UtmZone zone; double lon; double lat; } cases[] = {
		{ { 8, false }, -133.2, 54.9 }, { { 52, false }, 129.4, 35.1 }, { { 56, true }, 151.2, -33.9 } };
	for (const auto& [zone, lon, lat] : cases)
	{
		const UtmPoint there = geographicToUtm(zone, { lon, lat });
		const GeoPosition back = utmToGeographic(zone, there.easting, there.northing);
		EXPECT_NEAR(back.lon, lon, 5e-8);
		EXPECT_NEAR(back.lat, lat, 5e-8);
	}
}

TEST(CoverageUtm, GridKeepsEastingsAndNorthingsAndConvertsOnRequest)
{
	const GeoPosition position = gridPosition(noaaGrid, 1000, 500);
	EXPECT_EQ(position, utmToGeographic(*noaaGrid.utm, 576958.7877 + 16000, 6050944.4675 + 8000));
	const GridIndex back = gridCoordinates(noaaGrid, position);
	EXPECT_NEAR(back.col, 1000, 5e-4);
	EXPECT_NEAR(back.row, 500, 5e-4);

	EXPECT_EQ(gridCellAt(noaaGrid, gridPosition(noaaGrid, 1000.4, 499.6)), (GridCell{ 1000, 500 }));
	EXPECT_EQ(gridCellAt(noaaGrid, gridPosition(noaaGrid, -0.6, 10)), std::nullopt);
	// Inside the geographic bounds, yet outside the grid: the corner the grid convergence leaves empty.
	const GeoBounds bounds = gridBounds(noaaGrid);
	EXPECT_EQ(gridCellAt(noaaGrid, { bounds.minLon, bounds.maxLat }), std::nullopt);

	// The south edge sags between its corners: the bounds take the curve, not only the corners.
	const double cornerLat = std::min(gridPosition(noaaGrid, 0, 0).lat, gridPosition(noaaGrid, noaaGrid.cols - 1, 0).lat);
	EXPECT_LE(bounds.minLat, cornerLat);
	EXPECT_NEAR(gridCellSize(noaaGrid).lat, 16 / 111320.0, 5e-7);
}

TEST(CoverageUtm, HandsOutGridLinesInPiecesThatFollowThemOnTheChart)
{
	const auto line = gridLine(noaaGrid, -0.5, -0.5, noaaGrid.cols - 0.5, -0.5);
	EXPECT_EQ(line.size(), static_cast<std::size_t>(std::ceil(noaaGrid.cols / 32.0)) + 1);
	const GeoPosition chord{ (line.front().lon + line.back().lon) / 2, (line.front().lat + line.back().lat) / 2 };
	EXPECT_GT(std::abs(gridCoordinates(noaaGrid, chord).row + 0.5) * 16, 20);
	const GeoPosition middle{ (line[10].lon + line[11].lon) / 2, (line[10].lat + line[11].lat) / 2 };
	EXPECT_LT(std::abs(gridCoordinates(noaaGrid, middle).row + 0.5) * 16, 0.05);

	EXPECT_EQ(gridBlockRing(noaaGrid, -0.5, -0.5, 31.5, 31.5).size(), 4u);
	const RegularGrid geographic{ .originLon = 128, .originLat = 34, .spacingLon = 0.5, .spacingLat = 0.25, .cols = 300, .rows = 200 };
	const auto straight = gridLine(geographic, -0.5, -0.5, 299.5, -0.5);
	ASSERT_EQ(straight.size(), 2u);
	EXPECT_EQ(straight[0], (GeoPosition{ 127.75, 33.875 }));
	EXPECT_EQ(straight[1], (GeoPosition{ 277.75, 33.875 }));
}

TEST(CoverageUtm, MapsALatticeOntoTheGridWithinAFractionOfACell)
{
	const RegularGrid geographic{ .originLon = -133.8, .originLat = 54.6, .spacingLon = 0.00025, .spacingLat = 0.00014, .cols = 400, .rows = 300 };
	LatticeMapping mapping(geographic.cols, geographic.rows,
		[&](double x, double y) { return gridPosition(geographic, x, y); }, noaaGrid);
	std::vector<double> cols(static_cast<std::size_t>(geographic.cols));
	std::vector<double> rows(static_cast<std::size_t>(geographic.cols));
	for (const int y : { 0, 17, 299 })
	{
		mapping.line(y, cols, rows);
		for (const int x : { 0, 45, 399 })
		{
			const GridIndex exact = gridCoordinates(noaaGrid, gridPosition(geographic, x, y));
			EXPECT_LT(std::abs(cols[static_cast<std::size_t>(x)] - exact.col), 0.001);
			EXPECT_LT(std::abs(rows[static_cast<std::size_t>(x)] - exact.row), 0.001);
		}
	}
}
