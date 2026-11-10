#include <gtest/gtest.h>

#include "Coverage/DepthLines.h"

#include <algorithm>
#include <cmath>
#include <set>
#include <string>
#include <vector>

// Coverage backport CV-4 (Documents/TechDocs/2026-11-11-coverage-backport-plan.md): the S-98
// Appendix D lines and the valid-cell outline, with the cases of opens100-web
// `core/test/enhanced-safety-contour.test.ts`, `core/test/valid-grid-rings.test.ts` and
// `sdk/test/depth-compute.test.ts`.
using namespace s100::coverage;

namespace
{
	// S-98 Figure D-1-5, bottom row first (row 0 is the grid origin); 9999 is the fill value.
	constexpr float Fill = 9999;
	const std::vector<float> figureD15{
		Fill, Fill, 11.13f, 11.12f, 11.62f,
		Fill, 11.51f, 11.62f, 11.51f, 11.55f,
		11.65f, 11.65f, 11.75f, 11.45f, Fill,
		11.61f, 11.46f, 11.43f, 11.43f, Fill,
	};
	const RegularGrid unitGrid{ .originLon = 0, .originLat = 0, .spacingLon = 1, .spacingLat = 1, .cols = 5, .rows = 4 };

	struct Measure
	{
		double length = 0;
		std::size_t segments = 0;
	};

	// Total edge length in grid spacings, and the segment count.
	Measure measure(const GridEdges& edges)
	{
		Measure result;
		for (std::size_t i = 0; i + 3 < edges.size(); i += 4)
		{
			result.length += std::abs(edges[i + 2] - edges[i]) + std::abs(edges[i + 3] - edges[i + 1]);
		}
		result.segments = edges.size() / 4;
		return result;
	}
}

TEST(CoverageDepthLines, SafetyContourSelectsTheCommonEdgesOfFigureD15)
{
	const GridEdges edges = safetyContourEdges(unitGrid, figureD15, Fill, 11.5);
	// Three vertical and five horizontal unit edges; two horizontal pairs join into single segments.
	EXPECT_EQ(measure(edges).length, 8);
	EXPECT_EQ(measure(edges).segments, 6u);
	bool found = false;   // the edge between 11.61 and 11.46 in the top row
	for (std::size_t i = 0; i + 3 < edges.size(); i += 4)
	{
		found = found || (edges[i] == 0.5 && edges[i + 1] == 2.5 && edges[i + 2] == 0.5 && edges[i + 3] == 3.5);
	}
	EXPECT_TRUE(found);
	EXPECT_GT(measure(safetyContourEdges(unitGrid, figureD15, Fill, 11.7)).length, 0);
	EXPECT_EQ(measure(safetyContourEdges(unitGrid, figureD15, Fill, 0)).length, 0);
}

TEST(CoverageDepthLines, BoundaryOutlinesDataCellsAndLeavesOutTheWlaBoundary)
{
	const auto hasData = [](std::size_t i) { return !isNoData(figureD15[i], Fill); };
	EXPECT_EQ(measure(boundaryEdges(unitGrid, hasData)).length, 18);
	const std::set<std::size_t> adjusted{ 2, 3, 4 };   // the bottom row's data cells
	EXPECT_EQ(measure(boundaryEdges(unitGrid, hasData, [&](std::size_t i) { return !adjusted.contains(i); })).length, 13);
	EXPECT_EQ(measure(boundaryEdges(unitGrid, [&](std::size_t i) { return adjusted.contains(i); })).length, 8);
}

TEST(CoverageDepthLines, DepthLinesMergeTheContourAndOmitAnInactiveWlaBoundary)
{
	const RegularGrid grid{ .originLon = 10, .originLat = 50, .spacingLon = 1, .spacingLat = 1, .cols = 3, .rows = 2 };
	constexpr float F = -9999;
	const std::vector<float> depth{ 5, 40, F, 5, 40, 40 };
	const std::vector<std::uint8_t> applied(6, 0);
	const DepthLines lines = depthLines({ .grid = &grid, .depth = depth, .fillValue = F, .applied = applied, .appliedCount = 0,
		.esc = true, .safetyContour = 30, .wlaActive = false });
	// Two edges cross the 30 m contour, between cells 0|1 and 3|4 on the same grid line: one segment.
	ASSERT_TRUE(lines.safetyContour);
	EXPECT_EQ(lines.safetyContour->size(), 4u);
	ASSERT_TRUE(lines.s102Boundary);
	EXPECT_FALSE(lines.s102Boundary->empty());
	EXPECT_FALSE(lines.wlaBoundary);
	EXPECT_EQ(validGridRings(grid, depth, F).size(), 1u);
}

TEST(CoverageDepthLines, ValidRingsOutlineABlockWithAVertexPerCorner)
{
	const RegularGrid grid{ .originLon = 10, .originLat = 50, .spacingLon = 1, .spacingLat = 1, .cols = 5, .rows = 4 };
	constexpr float F = -9999;
	std::vector<float> values(20, F);
	for (int i = 0; i < 20; ++i)
	{
		const int r = i / 5;
		const int c = i % 5;
		if (r >= 1 && r <= 2 && c >= 1 && c <= 3) values[static_cast<std::size_t>(i)] = 1;
	}
	const auto rings = validGridRings(grid, values, F);
	ASSERT_EQ(rings.size(), 1u);
	EXPECT_EQ(rings[0].exterior, (std::vector<GeoPosition>{ { 10.5, 50.5 }, { 13.5, 50.5 }, { 13.5, 52.5 }, { 10.5, 52.5 } }));
}

TEST(CoverageDepthLines, ValidRingsGiveAHoleItsOwnRingAndKeepCornerTouchingRegionsApart)
{
	const RegularGrid grid{ .originLon = 10, .originLat = 50, .spacingLon = 1, .spacingLat = 1, .cols = 5, .rows = 4 };
	constexpr float F = -9999;
	std::vector<float> holed(20, 1);
	holed[7] = F;
	const auto rings = validGridRings(grid, holed, F);
	ASSERT_EQ(rings.size(), 2u);
	EXPECT_EQ(rings[0].exterior.size(), 4u);
	EXPECT_EQ(rings[1].exterior.size(), 4u);
	const auto hole = std::ranges::find_if(rings, [](const GridRing& ring) {
		return std::ranges::any_of(ring.exterior, [](const GeoPosition& p) { return p.lon == 11.5; });
	});
	ASSERT_NE(hole, rings.end());
	std::set<std::pair<double, double>> corners;
	for (const auto& p : hole->exterior) corners.insert({ p.lon, p.lat });
	EXPECT_EQ(corners, (std::set<std::pair<double, double>>{ { 11.5, 50.5 }, { 11.5, 51.5 }, { 12.5, 50.5 }, { 12.5, 51.5 } }));

	std::vector<float> diagonal(20, F);
	diagonal[0] = 1;
	diagonal[6] = 1;
	const auto apart = validGridRings(grid, diagonal, F);
	ASSERT_EQ(apart.size(), 2u);
	EXPECT_TRUE(std::ranges::all_of(apart, [](const GridRing& ring) { return ring.exterior.size() == 4; }));
	EXPECT_TRUE(validGridRings(grid, std::vector<float>(20, F), F).empty());
}

TEST(CoverageDepthLines, BoundariesFollowProjectedGridLines)
{
	// The layout of NOAA's 102US004AK5FL262267, cut to 100 x 3: south and north edges in four
	// pieces each (100 cells / 32), west and east in one.
	const RegularGrid grid{ .originLon = 576958.7877, .originLat = 6050944.4675, .spacingLon = 16, .spacingLat = 16, .cols = 100, .rows = 3,
		.utm = UtmZone{ 8, false } };
	const GridEdges edges = boundaryEdges(grid, [](std::size_t) { return true; });
	EXPECT_EQ(edges.size() / 4, 4u + 4u + 1u + 1u);
	const GeoPosition start = gridPosition(grid, -0.5, -0.5);
	EXPECT_EQ(edges[0], start.lon);
	EXPECT_EQ(edges[1], start.lat);
}

TEST(CoverageDepthLines, DepwlIsABlueAndBlackStaysBlack)
{
	const Rgb black = xyLToRgb(0.28, 0.31, 0);
	EXPECT_EQ(black.r + black.g + black.b, 0);
	const Rgb depwl = depwlColor("Day");
	EXPECT_EQ(depwl.b, 255);
	EXPECT_LT(depwl.r, depwl.g);
	EXPECT_EQ(safetyContourStyle().token, "DEPSC");
	EXPECT_EQ(wlaBoundaryStyle().dash.size(), 4u);
}
