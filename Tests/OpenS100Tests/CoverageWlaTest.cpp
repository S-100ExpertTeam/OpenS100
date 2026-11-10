#include <gtest/gtest.h>

#include "TestPaths.h"
#include "Coverage/DepthLines.h"
#include "Coverage/Hdf5Coverage.h"
#include "Coverage/WaterLevelAdjustment.h"

#include <cmath>
#include <filesystem>
#include <vector>

// Coverage backport CV-5 (Documents/TechDocs/2026-11-11-coverage-backport-plan.md): water level
// adjustment, with the cases of opens100-web `core/test/water-level-adjustment.test.ts` and
// `hdf5/test/water-level-adjustment.test.ts`.
using namespace s100::coverage;
using namespace std::chrono;

namespace
{
	TimePoint at(int hh, int mm)
	{
		return TimePoint{ sys_days{ 2021y / November / 8 } + hours(hh) + minutes(mm) };
	}

	CoverageComponent component(std::string name, double fill, std::optional<double> lower = std::nullopt, std::optional<double> upper = std::nullopt)
	{
		return CoverageComponent{ .name = name, .label = name, .uom = "metres", .fillValue = fill, .lower = lower, .upper = upper };
	}

	// A single-feature coverage; values[time][component] holds the grid values.
	class FakeCoverage final : public CoverageSource
	{
	public:
		FakeCoverage(std::string productId, std::string code, RegularGrid grid, std::vector<CoverageComponent> components,
			std::vector<std::optional<TimePoint>> times, std::vector<std::vector<std::vector<float>>> values,
			std::map<std::string, MetadataValue, std::less<>> metadata)
			: values_(std::move(values))
		{
			dataset_.name = productId;
			dataset_.productId = productId;
			dataset_.metadata = std::move(metadata);
			dataset_.bounds = gridBounds(grid);
			CoverageFeature feature{ .id = code + "/" + code + ".01", .code = code, .label = code, .components = std::move(components), .grid = grid };
			feature.compound = feature.components.size() > 1;
			for (std::size_t i = 0; i < times.size(); ++i)
			{
				feature.timeGroups.push_back({ "Group_00" + std::to_string(i + 1), times[i] });
			}
			dataset_.features.push_back(std::move(feature));
		}

		const CoverageDataset& dataset() const noexcept override { return dataset_; }
		Values readValues(std::string_view, const CoverageSelection& selection) const override
		{
			return std::make_shared<const std::vector<float>>(values_.at(selection.timeIndex).at(selection.component));
		}

		CoverageDataset dataset_;

	private:
		std::vector<std::vector<std::vector<float>>> values_;
	};

	std::map<std::string, MetadataValue, std::less<>> s100Datum(std::optional<double> verticalCS = std::nullopt)
	{
		std::map<std::string, MetadataValue, std::less<>> metadata{ { "verticalDatumReference", 1.0 }, { "verticalDatum", 11.0 }, { "horizontalCRS", 4326.0 } };
		if (verticalCS) metadata["verticalCS"] = *verticalCS;
		return metadata;
	}

	std::shared_ptr<FakeCoverage> bathymetry(std::vector<float> depths, std::vector<float> uncertainties, const RegularGrid& grid)
	{
		return std::make_shared<FakeCoverage>("S-102", "BathymetryCoverage", grid,
			std::vector<CoverageComponent>{ component("depth", 1000000), component("uncertainty", 1000000) },
			std::vector<std::optional<TimePoint>>{ std::nullopt }, std::vector<std::vector<std::vector<float>>>{ { std::move(depths), std::move(uncertainties) } },
			s100Datum());
	}

	std::shared_ptr<FakeCoverage> waterLevel(std::vector<std::vector<float>> heights, const RegularGrid& grid,
		std::map<std::string, MetadataValue, std::less<>> metadata = s100Datum(6499), std::vector<std::vector<float>> uncertainties = {})
	{
		std::vector<CoverageComponent> components{ component("waterLevelHeight", -9999, -99.99, 99.99) };
		if (!uncertainties.empty()) components.push_back(component("uncertainty", -1));
		std::vector<std::optional<TimePoint>> times;
		std::vector<std::vector<std::vector<float>>> values;
		for (std::size_t i = 0; i < heights.size(); ++i)
		{
			times.push_back(at(12, 34) + minutes(30 * static_cast<int>(i)));
			values.push_back(uncertainties.empty() ? std::vector<std::vector<float>>{ heights[i] } : std::vector<std::vector<float>>{ heights[i], uncertainties[i] });
		}
		return std::make_shared<FakeCoverage>("S-104", "WaterLevel", grid, std::move(components), std::move(times), std::move(values), std::move(metadata));
	}

	const RegularGrid one{ .originLon = 0, .originLat = 0, .spacingLon = 1, .spacingLat = 1, .cols = 1, .rows = 1 };
	constexpr const char* bathymetryId = "BathymetryCoverage/BathymetryCoverage.01";

	WlaReport adjust(WaterLevelAdjustedCoverage& s102, const DepthAdjustmentSettings& settings, const CoverageSource& s104)
	{
		const CoverageSource* sources[] = { &s104 };
		return s102.update(settings, sources);
	}
}

TEST(CoverageWla, Option1TakesTheRecordAtTheInstantOrTheTwoEitherSide)
{
	// Figure D-2-8: records every 30 minutes from 06:00 to 09:00.
	std::vector<CoverageTimeGroup> groups;
	for (int m : { 0, 30, 60, 90, 120, 150, 180 }) groups.push_back({ "Group", at(6, 0) + minutes(m) });
	EXPECT_EQ(selectWaterLevelRecords(groups, WlaMethod::instant(at(7, 24))), (std::vector<std::size_t>{ 2, 3 }));
	EXPECT_EQ(selectWaterLevelRecords(groups, WlaMethod::instant(at(7, 30))), (std::vector<std::size_t>{ 3 }));
	EXPECT_EQ(selectWaterLevelRecords(groups, WlaMethod::instant(at(5, 59))), std::nullopt);
	EXPECT_EQ(selectWaterLevelRecords(groups, WlaMethod::instant(at(9, 1))), std::nullopt);
	// Option 2 adds the records immediately before the start and after the end (Figure D-2-10).
	EXPECT_EQ(selectWaterLevelRecords(groups, WlaMethod::period(at(7, 10), at(8, 20))), (std::vector<std::size_t>{ 2, 3, 4, 5 }));
	EXPECT_EQ(selectWaterLevelRecords(groups, WlaMethod::period(at(6, 40), at(6, 50))), (std::vector<std::size_t>{ 1, 2 }));
	EXPECT_EQ(selectWaterLevelRecords(groups, WlaMethod::period(at(6, 10), at(6, 50))), (std::vector<std::size_t>{ 0, 1, 2 }));
	EXPECT_EQ(selectWaterLevelRecords(groups, WlaMethod::period(at(5, 50), at(6, 50))), std::nullopt);
}

TEST(CoverageWla, ReproducesTheTable10Example)
{
	WaterLevelAdjustedCoverage s102(bathymetry({ 10 }, { 0.42f }, one));
	const auto s104 = waterLevel({ { 0.32f } }, one, s100Datum(6499), { { 0.04f } });
	const WlaMethod instant = WlaMethod::instant(at(12, 34));

	EXPECT_EQ(adjust(s102, { .uncertainty = false, .wla = instant }, *s104), (WlaReport{ 1, {} }));
	EXPECT_NEAR((*s102.readValues(bathymetryId, { .timeIndex = 0, .component = 0 }))[0], 10.32, 5e-6);
	(void)adjust(s102, { .uncertainty = true, .wla = instant }, *s104);
	EXPECT_NEAR((*s102.adjustment(bathymetryId)->depth)[0], 9.86, 5e-6);
	// Uncertainty alone still biases the depth, without WLA.
	(void)adjust(s102, { .uncertainty = true, .wla = WlaMethod::off() }, *s104);
	EXPECT_EQ(s102.adjustment(bathymetryId)->appliedCount, 0u);
	EXPECT_NEAR((*s102.adjustment(bathymetryId)->depth)[0], 9.58, 5e-6);
	// The base keeps the unadjusted depth for the pick report.
	EXPECT_EQ((*s102.base().readValues(bathymetryId, { .timeIndex = 0, .component = 0 }))[0], 10.0f);
}

TEST(CoverageWla, NegatesLevelsEncodedPositiveDown)
{
	WaterLevelAdjustedCoverage s102(bathymetry({ 10 }, { 0 }, one));
	(void)adjust(s102, { .wla = WlaMethod::instant(at(12, 34)) }, *waterLevel({ { 0.32f } }, one, s100Datum(6498)));
	EXPECT_NEAR((*s102.adjustment(bathymetryId)->depth)[0], 9.68, 5e-6);
}

TEST(CoverageWla, AdjustsByTheShoalestIntersectingS104Cell)
{
	// Figure D-2-9: S-102 point x at the corner shared by four S-104 points.
	WaterLevelAdjustedCoverage s102(bathymetry({ 20, 20, 20, 20 }, { 0, 0, 0, 0 },
		{ .originLon = 0.5, .originLat = 0.5, .spacingLon = 0.5, .spacingLat = 0.5, .cols = 2, .rows = 2 }));
	const auto s104 = waterLevel({ { 1.3f, 1.1f, 1.2f, 1.4f }, { 1.0f, 1.2f, 1.2f, 1.2f } },
		{ .originLon = 0, .originLat = 0, .spacingLon = 1, .spacingLat = 1, .cols = 2, .rows = 2 });
	(void)adjust(s102, { .wla = WlaMethod::instant(at(12, 34)) }, *s104);
	EXPECT_NEAR(s102.adjustment(bathymetryId)->waterLevel[0], 1.1, 5e-6);
	EXPECT_NEAR(s102.adjustment(bathymetryId)->waterLevel[3], 1.4, 5e-6);
	// Between two records the shoalest of both is used.
	(void)adjust(s102, { .wla = WlaMethod::instant(at(12, 50)) }, *s104);
	EXPECT_NEAR(s102.adjustment(bathymetryId)->waterLevel[0], 1.0, 5e-6);
	EXPECT_NEAR(s102.adjustment(bathymetryId)->waterLevel[3], 1.2, 5e-6);
}

TEST(CoverageWla, RefusesIncompatibleDatumsAndTimesOutsideTheSeries)
{
	WaterLevelAdjustedCoverage s102(bathymetry({ 10 }, { 0 }, one));
	const auto otherDatum = waterLevel({ { 0.32f } }, one,
		{ { "verticalDatumReference", std::string("s100VerticalDatum") }, { "verticalDatum", 15.0 }, { "verticalCS", 6499.0 } });
	const WlaMethod instant = WlaMethod::instant(at(12, 34));
	EXPECT_EQ(adjust(s102, { .wla = instant }, *otherDatum), (WlaReport{ 0, { WlaIssue::IncompatibleDatums } }));
	EXPECT_EQ(adjust(s102, { .wla = instant, .ignoreIncompatibleDatums = true }, *otherDatum), (WlaReport{ 1, { WlaIssue::IncompatibleDatums } }));
	EXPECT_EQ(adjust(s102, { .wla = WlaMethod::instant(at(13, 0)) }, *waterLevel({ { 0.32f } }, one)), (WlaReport{ 0, { WlaIssue::TimeNotCovered } }));
	EXPECT_EQ(verticalDatumName(verticalDatumOf(otherDatum->dataset())), "approximateMeanLowerLowWater (15)");
}

TEST(CoverageWla, AdjustsAUtmS102GridByAGeographicS104Grid)
{
	// 4 x 4 cells of 500 m on zone 8N over the point where four S-104 cells meet, 54.75 N 133.5 W.
	const RegularGrid s102Grid{ .originLon = 595914.8, .originLat = 6067384.4, .spacingLon = 500, .spacingLat = 500, .cols = 4, .rows = 4,
		.utm = UtmZone{ 8, false } };
	const RegularGrid s104Grid{ .originLon = -133.51, .originLat = 54.74, .spacingLon = 0.02, .spacingLat = 0.02, .cols = 2, .rows = 2 };
	WaterLevelAdjustedCoverage s102(bathymetry(std::vector<float>(16, 10), std::vector<float>(16, 0), s102Grid));
	const auto s104 = waterLevel({ { 0.1f, 0.2f, 0.3f, 0.4f } }, s104Grid);
	EXPECT_EQ(adjust(s102, { .wla = WlaMethod::instant(at(12, 34)) }, *s104), (WlaReport{ 16, {} }));

	const auto& depth = *s102.adjustment(bathymetryId)->depth;
	const float levels[] = { 0.1f, 0.2f, 0.3f, 0.4f };
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			// The S-104 cells the S-102 cell's corners fall in; the shoalest of their levels.
			float shoalest = 1e9f;
			for (const auto& [dc, dr] : { std::pair{ -0.5, -0.5 }, std::pair{ 0.5, -0.5 }, std::pair{ -0.5, 0.5 }, std::pair{ 0.5, 0.5 } })
			{
				const auto corner = gridCellAt(s104Grid, gridPosition(s102Grid, col + dc, row + dr));
				ASSERT_TRUE(corner);
				shoalest = std::min(shoalest, levels[corner->row * 2 + corner->col]);
			}
			EXPECT_NEAR(depth[static_cast<std::size_t>(row * 4 + col)], 10 + shoalest, 5e-6) << col << "," << row;
		}
	}
	EXPECT_NEAR(depth[0], 10.1, 5e-6);
	EXPECT_NEAR(depth[15], 10.4, 5e-6);
}

TEST(CoverageWla, LeavesFillValuesAndCellsWithoutLevelsUnadjusted)
{
	WaterLevelAdjustedCoverage s102(bathymetry({ 1000000, 12 }, { 0, 0 }, { .originLon = 0, .originLat = 0, .spacingLon = 1, .spacingLat = 1, .cols = 2, .rows = 1 }));
	(void)adjust(s102, { .wla = WlaMethod::instant(at(12, 34)) }, *waterLevel({ { -9999 } }, one));
	EXPECT_EQ(*s102.adjustment(bathymetryId)->depth, (std::vector<float>{ 1000000, 12 }));
	EXPECT_EQ(s102.adjustment(bathymetryId)->appliedCount, 0u);
}

TEST(CoverageWla, FormatsTheIndicationAndDetectsOverlaps)
{
	EXPECT_EQ(wlaIndication(WlaMethod::instant(at(12, 34))), "WLA 12:34 08 Nov 2021");
	EXPECT_EQ(wlaIndication(WlaMethod::period(at(14, 56), at(12, 34))), "WLA from 12:34 08 Nov 2021 to 14:56 08 Nov 2021");
	EXPECT_EQ(wlaIndication(WlaMethod::off()), std::nullopt);

	auto a = bathymetry({ 1, 1 }, { 0, 0 }, { .originLon = 0, .originLat = 0, .spacingLon = 0.001, .spacingLat = 0.001, .cols = 2, .rows = 2 });
	a->dataset_.bounds = { 0, 0, 0.5, 0.5 };
	const auto b = bathymetry({ 1 }, { 0 }, { .originLon = 0.0005, .originLat = 0.0005, .spacingLon = 0.001, .spacingLat = 0.001, .cols = 500, .rows = 500 });
	EXPECT_TRUE(coveragesOverlap(a->dataset(), b->dataset()));
	auto c = bathymetry({ 1, 1 }, { 0, 0 }, { .originLon = 0, .originLat = 0, .spacingLon = 0.001, .spacingLat = 0.001, .cols = 2, .rows = 2 });
	c->dataset_.bounds = { 0.5005, 0, 1, 0.5 };
	EXPECT_FALSE(coveragesOverlap(a->dataset(), c->dataset()));
}

TEST(CoverageWla, TakesOnlyTheLevelOfTheCellUnderADepthCell)
{
	// 0.01 degree spacing: the cell edges are not exact in floating point.
	const RegularGrid grid{ .originLon = 8.005, .originLat = 53.805, .spacingLon = 0.01, .spacingLat = 0.01, .cols = 3, .rows = 2 };
	const std::vector<float> raw{ 5, 6, 7, 8, 9, 10 };
	const float nan = std::numeric_limits<float>::quiet_NaN();
	const std::vector<WaterLevelGrid> grids{ { grid, { 1, 1, 1, nan, nan, nan } } };
	const DepthAdjustment adjustment = adjustDepths({ .grid = &grid, .raw = raw, .fillValue = -9999, .grids = grids });
	EXPECT_EQ(adjustment.applied, (std::vector<std::uint8_t>{ 1, 1, 1, 0, 0, 0 }));
	EXPECT_EQ(*adjustment.depth, (std::vector<float>{ 6, 7, 8, 8, 9, 10 }));
}

TEST(CoverageWla, AdjustsTheKoreanS102ByTheKoreanS104)
{
	// opens100-web hdf5/test/water-level-adjustment.test.ts, on the KOR TDS pair over Busan.
	const auto s102File = testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-102" / L"S-102" / L"102KR00T4G3B3000026.h5";
	const auto s104File = testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-104" / L"S-104" / L"104KR00KR4_MOHID2K_G3B30_20260330_F36.h5";
	if (!std::filesystem::exists(s102File) || !std::filesystem::exists(s104File))
	{
		GTEST_SKIP() << "KOR S-102/S-104 not available";
	}
	std::shared_ptr<const CoverageSource> raw = Hdf5Coverage::open(s102File);
	const auto s104 = Hdf5Coverage::open(s104File);
	EXPECT_EQ(verticalDatumName(verticalDatumOf(raw->dataset())), "nearlyLowestLowWater (11)");
	EXPECT_EQ(verticalDatumName(verticalDatumOf(s104->dataset())), "approximateMeanLowerLowWater (15)");

	WaterLevelAdjustedCoverage s102(raw);
	const TimePoint first = *s104->dataset().features.front().timeGroups.front().time;
	const CoverageSource* sources[] = { s104.get() };
	const WlaReport refused = s102.update({ .wla = WlaMethod::instant(first) }, sources);
	EXPECT_EQ(refused, (WlaReport{ 0, { WlaIssue::IncompatibleDatums } }));
	const WlaReport forced = s102.update({ .wla = WlaMethod::instant(first), .ignoreIncompatibleDatums = true }, sources);
	EXPECT_GT(forced.applied, 0u);
	std::cout << "[ wla ] KOR S-102 cells adjusted by S-104 at its first record: " << forced.applied << std::endl;

	// The safety contour of the adjusted grid is not empty.
	const CoverageFeature& bathymetry = raw->dataset().features.front();
	const auto depth = s102.readValues(bathymetry.id, { .timeIndex = 0, .component = 0 });
	ASSERT_TRUE(depth);
	EXPECT_FALSE(safetyContourEdges(bathymetry.grid, *depth, bathymetry.components[0].fillValue, 30).empty());
}
