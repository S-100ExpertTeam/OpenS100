#include <gtest/gtest.h>

#include "TestPaths.h"
#include "Coverage/Hdf5Coverage.h"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <set>

// Coverage backport CV-2 (Documents/TechDocs/2026-11-11-coverage-backport-plan.md): the S-100
// Part 10c HDF5 reader, with the expectations of opens100-web `packages/hdf5/test/read-hdf5.test.ts`
// on the same files where they are at hand (the web repository's viewer data), and the KOR TDS.
using namespace s100::coverage;
namespace fs = std::filesystem;

namespace
{
	fs::path webData(const char* name)
	{
		return testpaths::webDataRoot() / name;
	}

	fs::path korS102()
	{
		return testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-102" / L"S-102" / L"102KR00T4G3B3000026.h5";
	}

	fs::path korS104()
	{
		return testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-104" / L"S-104" / L"104KR00KR4_MOHID2K_G3B30_20260330_F36.h5";
	}

#define SKIP_UNLESS_EXISTS(path) \
	if (!fs::exists(path)) { GTEST_SKIP() << "test data not found: " << (path).string(); }
}

TEST(CoverageHdf5, RejectsAFileThatIsNotHdf5)
{
	const fs::path bogus = testpaths::tempDir() / "bogus.h5";
	{
		std::ofstream out(bogus, std::ios::binary);
		out << "not hdf5";
	}
	try
	{
		(void)Hdf5Coverage::open(bogus);
		FAIL() << "opened a file that is not HDF5";
	}
	catch (const std::runtime_error& error)
	{
		EXPECT_NE(std::string(error.what()).find("bogus.h5"), std::string::npos) << error.what();
	}
}

TEST(CoverageHdf5, ReadsKoreanS102Grid)
{
	SKIP_UNLESS_EXISTS(korS102());
	const auto coverage = Hdf5Coverage::open(korS102());
	const CoverageDataset& dataset = coverage->dataset();
	EXPECT_EQ(dataset.productId, "S-102");
	EXPECT_TRUE(coverage->skipped().empty() || std::ranges::none_of(coverage->skipped(),
		[](const auto& skipped) { return skipped.code.starts_with("BathymetryCoverage"); }));

	const CoverageFeature* bathymetry = dataset.feature("BathymetryCoverage/BathymetryCoverage.01");
	ASSERT_NE(bathymetry, nullptr);
	ASSERT_EQ(bathymetry->components.size(), 2u);
	EXPECT_EQ(bathymetry->components[0].name, "depth");
	EXPECT_EQ(bathymetry->components[1].name, "uncertainty");
	EXPECT_EQ(bathymetry->components[0].fillValue, 1000000);
	EXPECT_TRUE(bathymetry->compound);
	ASSERT_EQ(bathymetry->timeGroups.size(), 1u);
	EXPECT_FALSE(bathymetry->timeGroups[0].time) << "S-102 has no valid time";
	EXPECT_FALSE(bathymetry->grid.utm);
	EXPECT_EQ(bathymetry->grid.cols * bathymetry->grid.rows, 501 * 501);
	EXPECT_TRUE(dataset.bounds.intersects(GeoBounds{ 129.0, 35.0, 129.5, 35.5 }));

	const Values depth = coverage->readValues(bathymetry->id, { .timeIndex = 0, .component = 0 });
	ASSERT_TRUE(depth);
	ASSERT_EQ(depth->size(), bathymetry->grid.nodeCount());
	const auto range = valueRange(*depth, bathymetry->components[0].fillValue);
	ASSERT_TRUE(range);
	EXPECT_NEAR(range->min, -1.2, 0.01);
	EXPECT_NEAR(range->max, 221.51, 0.01);
	const auto withDepth = std::ranges::count_if(*depth, [&](float value) { return !isNoData(value, bathymetry->components[0].fillValue); });
	EXPECT_EQ(withDepth, 138200);

	// The other member came with the same read; asking again is served from the cache.
	const Values uncertainty = coverage->readValues(bathymetry->id, { .timeIndex = 0, .component = 1 });
	ASSERT_TRUE(uncertainty);
	EXPECT_EQ(coverage->readValues(bathymetry->id, { .timeIndex = 0, .component = 0 }), depth);
	EXPECT_FALSE(coverage->readValues(bathymetry->id, { .timeIndex = 1 }));
	EXPECT_FALSE(coverage->readValues("Nothing/Nothing.01", {}));
}

TEST(CoverageHdf5, MovesTheNodesToTheDataPointsOfAnOffsetFile)
{
	// KHOA: dataOffsetCode 5, the data point at the centre of the cell whose corner is the grid point.
	const fs::path file = webData("102KR00N4F2K3000026.h5");
	SKIP_UNLESS_EXISTS(file);
	const auto coverage = Hdf5Coverage::open(file);
	const CoverageFeature* bathymetry = coverage->dataset().feature("BathymetryCoverage/BathymetryCoverage.01");
	ASSERT_NE(bathymetry, nullptr);
	EXPECT_NEAR(bathymetry->grid.originLon, 126.0015, 5e-10);
	EXPECT_NEAR(bathymetry->grid.originLat, 37.0015, 5e-10);
	EXPECT_EQ(bathymetry->grid.spacingLon, 0.001);
	EXPECT_EQ(bathymetry->grid.spacingLat, 0.001);
	EXPECT_EQ(bathymetry->grid.cols, 499);
	EXPECT_EQ(bathymetry->grid.rows, 500);
}

TEST(CoverageHdf5, ReadsS104HeightsAndTheEnumeratedTrend)
{
	const fs::path file = webData("104KR00KR4_MOHID2K_F4N30_20260330_F36.h5");
	SKIP_UNLESS_EXISTS(file);
	const auto coverage = Hdf5Coverage::open(file);
	const CoverageDataset& dataset = coverage->dataset();
	EXPECT_EQ(dataset.productId, "S-104");
	ASSERT_FALSE(dataset.features.empty());
	const CoverageFeature& level = dataset.features.front();
	ASSERT_EQ(level.components.size(), 2u);
	EXPECT_EQ(level.components[0].name, "waterLevelHeight");
	EXPECT_EQ(level.components[1].name, "waterLevelTrend");
	ASSERT_TRUE(level.components[0].datasetRange);
	EXPECT_NEAR(level.components[0].datasetRange->min, 0.52, 5e-7);
	EXPECT_NEAR(level.components[0].datasetRange->max, 2.76, 5e-7);
	EXPECT_FALSE(level.components[1].datasetRange);
	ASSERT_GT(level.timeGroups.size(), 3u);
	EXPECT_TRUE(level.timeGroups[0].time) << "S-104 records carry their time";

	const Values height = coverage->readValues(level.id, { .timeIndex = 3, .component = 0 });
	const Values trend = coverage->readValues(level.id, { .timeIndex = 3, .component = 1 });
	ASSERT_TRUE(height && trend);
	EXPECT_EQ(height->size(), level.grid.nodeCount());
	const std::set<float> trends(trend->begin(), trend->end());
	EXPECT_TRUE(std::ranges::all_of(trends, [](float value) { return value == 0 || value == 1 || value == 2 || value == 3; }));

	// No data offset stated: the nodes stay on the grid points.
	const auto west = dataset.numberAttribute("westBoundLongitude");
	ASSERT_TRUE(west);
	EXPECT_NEAR(level.grid.originLon, *west, 5e-6);
}

TEST(CoverageHdf5, DropsAStatedRangeThatHoldsTheFillValue)
{
	const fs::path file = webData("104KR00KR4_MOHID2K_F2K20_20260330_F36.h5");
	SKIP_UNLESS_EXISTS(file);
	const auto coverage = Hdf5Coverage::open(file);
	const CoverageComponent& height = coverage->dataset().features.front().components.front();
	ASSERT_TRUE(height.lower);
	EXPECT_NEAR(*height.lower, -99.99, 1e-9);
	EXPECT_FALSE(height.datasetRange);
}

TEST(CoverageHdf5, ReadsTheKoreanS104TimeSeriesAndVerticalDatum)
{
	SKIP_UNLESS_EXISTS(korS104());
	const auto coverage = Hdf5Coverage::open(korS104());
	const CoverageDataset& dataset = coverage->dataset();
	const CoverageFeature& level = dataset.features.front();
	ASSERT_FALSE(level.timeGroups.empty());
	using namespace std::chrono;
	EXPECT_EQ(level.timeGroups[0].time, TimePoint{ sys_days{ 2026y / March / 29 } + 15h });
	EXPECT_EQ(dataset.numberAttribute("verticalCS"), 6499);
	EXPECT_TRUE(dataset.numberAttribute("verticalDatum"));
}
