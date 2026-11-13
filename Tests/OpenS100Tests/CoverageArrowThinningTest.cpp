#include <gtest/gtest.h>

#include "Coverage/ArrowThinning.h"
#include "Coverage/CoverageLookup.h"

#include <cmath>
#include <limits>
#include <set>
#include <utility>

// Thinning of S-111 surface current arrows (S-111 Edition 2.0.0 clause 9.3.2, equations 9.2 and
// 9.3) and the arrow length it is computed from (clause 9.2.4, equation 9.1).
using namespace s100::coverage;

namespace
{
	// A chart of `scale` screen units to the degree, north up.
	ToScreen flatChart(double scale)
	{
		return [scale](const GeoPosition& position) { return ScreenPoint{ position.lon * scale, -position.lat * scale }; };
	}

	const RegularGrid grid{ .originLon = 126, .originLat = 34, .spacingLon = 0.01, .spacingLat = 0.01, .cols = 101, .rows = 51 };
}

TEST(ArrowThinning, ArrowLengthFollowsEquation91)
{
	// H = Href * min(max(Slow, S), Shigh) / Sref with Href 10 mm, Sref 5 kn, Slow 2 kn, Shigh 13 kn.
	EXPECT_DOUBLE_EQ(arrowLength(5), 10);
	EXPECT_DOUBLE_EQ(arrowLength(3), 6);
	EXPECT_DOUBLE_EQ(arrowLength(0.3), 4) << "no shorter than at 2 knots";
	EXPECT_DOUBLE_EQ(arrowLength(2), 4);
	EXPECT_DOUBLE_EQ(arrowLength(13), 26);
	EXPECT_DOUBLE_EQ(arrowLength(20), 26) << "no longer than at 13 knots";
	EXPECT_DOUBLE_EQ(maxArrowLength(), 26);
	EXPECT_DOUBLE_EQ(arrowLength(std::numeric_limits<double>::quiet_NaN()), 4);
	EXPECT_DOUBLE_EQ(arrowLength(-1), 4);

	const ArrowScale half{ .symbolLength = 5, .referenceSpeed = 10 };
	EXPECT_DOUBLE_EQ(arrowLength(10, half), 5);
	EXPECT_DOUBLE_EQ(arrowScaleFactor(20, half), 2);
}

TEST(ArrowThinning, FactorFollowsEquation93)
{
	// Every node while L / D <= 0.5.
	EXPECT_EQ(thinningFactor(100, 26), 1);
	EXPECT_EQ(thinningFactor(52, 26), 1) << "the ratio is exactly R_max";
	// n = 1 + fix(L / (D * 0.5)).
	EXPECT_EQ(thinningFactor(51.9, 26), 2);
	EXPECT_EQ(thinningFactor(26, 26), 3);
	EXPECT_EQ(thinningFactor(10, 26), 6);   // 1 + fix(5.2)
	EXPECT_EQ(thinningFactor(1, 26), 53);
	EXPECT_EQ(thinningFactor(10, 26, 1.0), 3);   // 1 + fix(2.6)

	// The constraint of equation 9.2 holds for the factor returned, and for no smaller one.
	for (const double diagonal : { 0.37, 1.0, 3.3, 7.0, 12.9, 13.0, 25.0, 51.0, 52.0, 80.0 })
	{
		for (const double length : { 4.0, 10.0, 26.0 })
		{
			const int n = thinningFactor(diagonal, length);
			EXPECT_LE(length / (n * diagonal), MaxArrowToSpacingRatio) << diagonal << " " << length;
			if (n > 1)
			{
				EXPECT_GE(length / ((n - 1) * diagonal), MaxArrowToSpacingRatio - 1e-12) << diagonal << " " << length;
				EXPECT_GT(length / diagonal, MaxArrowToSpacingRatio) << "thinned only when every node would not do";
			}
		}
	}
}

TEST(ArrowThinning, FactorOfDegenerateInput)
{
	const int largest = std::numeric_limits<int>::max();
	EXPECT_EQ(thinningFactor(10, 0), 1) << "no arrow, nothing to overlap";
	EXPECT_EQ(thinningFactor(10, std::numeric_limits<double>::quiet_NaN()), 1);
	EXPECT_EQ(thinningFactor(0, 26), largest) << "all nodes on one point";
	EXPECT_EQ(thinningFactor(std::numeric_limits<double>::quiet_NaN(), 26), largest);
	EXPECT_EQ(thinningFactor(1e-300, 26), largest);
}

TEST(ArrowThinning, NodeDiagonalIsMeasuredOnTheScreen)
{
	// 0.01 degree each way at 1000 units to the degree: 10 by 10.
	EXPECT_NEAR(nodeDiagonal(grid, flatChart(1000)), std::hypot(10, 10), 1e-9);
	EXPECT_NEAR(nodeDiagonal(grid, flatChart(100)), std::hypot(1, 1), 1e-9);
	// Outside the grid, the nearest part of it; the spacing is the same everywhere here.
	EXPECT_NEAR(nodeDiagonal(grid, flatChart(1000), GeoPosition{ 0, 0 }), std::hypot(10, 10), 1e-9);
	EXPECT_NEAR(nodeDiagonal(grid, flatChart(1000), GeoPosition{ 200, 80 }), std::hypot(10, 10), 1e-9);

	// Where the chart stretches with latitude, the spacing is taken where asked.
	const ToScreen stretched = [](const GeoPosition& position) { return ScreenPoint{ position.lon * 1000, -position.lat * position.lat * 10 }; };
	const double south = nodeDiagonal(grid, stretched, GeoPosition{ 126.5, 34.0 });
	const double north = nodeDiagonal(grid, stretched, GeoPosition{ 126.5, 34.5 });
	EXPECT_GT(north, south);

	// One row: measured to where the diagonal neighbour would be.
	const RegularGrid oneRow{ .originLon = 0, .originLat = 0, .spacingLon = 1, .spacingLat = 2, .cols = 5, .rows = 1 };
	EXPECT_NEAR(nodeDiagonal(oneRow, flatChart(1)), std::hypot(1, 2), 1e-12);
	EXPECT_EQ(nodeDiagonal(RegularGrid{}, flatChart(1)), 0);
}

TEST(ArrowThinning, ThinnedNodesTakeEveryNthColumnAndRow)
{
	const ThinnedNodes all = thinnedNodes(grid, 1);
	EXPECT_EQ(all, (ThinnedNodes{ .factor = 1, .firstCol = 0, .firstRow = 0, .cols = 101, .rows = 51 }));
	EXPECT_EQ(all.count(), 101u * 51u);

	const ThinnedNodes fourth = thinnedNodes(grid, 4);
	EXPECT_EQ(fourth, (ThinnedNodes{ .factor = 4, .firstCol = 0, .firstRow = 0, .cols = 26, .rows = 13 }));
	EXPECT_EQ(fourth.col(25), 100);
	EXPECT_EQ(fourth.row(12), 48);

	const ThinnedNodes seventh = thinnedNodes(grid, 7);
	EXPECT_EQ(seventh.cols, 15);   // 0, 7, ..., 98
	EXPECT_EQ(seventh.rows, 8);    // 0, 7, ..., 49

	EXPECT_EQ(thinnedNodes(grid, 0).factor, 1) << "a factor below 1 counts as 1";
	EXPECT_EQ(thinnedNodes(grid, std::numeric_limits<int>::max()).count(), 1u) << "one node when the grid is a point on the screen";
	EXPECT_TRUE(thinnedNodes(RegularGrid{}, 3).empty());
}

TEST(ArrowThinning, ThinnedNodesKeepTheAnchorsColumnAndRow)
{
	// S-111 9.3.2: the row and column with the maximum vector are drawn.
	const ThinnedNodes nodes = thinnedNodes(grid, 5, {}, GridCell{ .col = 37, .row = 14 });
	EXPECT_EQ(nodes.firstCol, 2);
	EXPECT_EQ(nodes.firstRow, 4);
	EXPECT_EQ(nodes.cols, 20);   // 2, 7, ..., 97
	EXPECT_EQ(nodes.rows, 10);   // 4, 9, ..., 49
	EXPECT_EQ((37 - nodes.firstCol) % nodes.factor, 0);
	EXPECT_EQ((14 - nodes.firstRow) % nodes.factor, 0);

	// A factor larger than the grid still leaves the anchor.
	const ThinnedNodes one = thinnedNodes(grid, 1000, {}, GridCell{ .col = 37, .row = 14 });
	EXPECT_EQ(one, (ThinnedNodes{ .factor = 1000, .firstCol = 37, .firstRow = 14, .cols = 1, .rows = 1 }));
}

TEST(ArrowThinning, ThinnedNodesAreCutToTheView)
{
	// Columns 20..40 and rows 10..20, one drawn node (the factor) beyond each edge.
	const GeoBounds view{ .minLon = 126.20, .minLat = 34.10, .maxLon = 126.40, .maxLat = 34.20 };
	const ThinnedNodes nodes = thinnedNodes(grid, 4, view);
	EXPECT_EQ(nodes.factor, 4);
	EXPECT_EQ(nodes.firstCol, 16);
	EXPECT_EQ(nodes.col(nodes.cols - 1), 44);
	EXPECT_EQ(nodes.firstRow, 8);
	EXPECT_EQ(nodes.row(nodes.rows - 1), 24);

	// The nodes in the view are the same ones as without it: panning does not move the arrows.
	const ThinnedNodes whole = thinnedNodes(grid, 4);
	std::set<std::pair<int, int>> everywhere;
	for (int j = 0; j < whole.rows; ++j)
	{
		for (int i = 0; i < whole.cols; ++i) everywhere.insert({ whole.col(i), whole.row(j) });
	}
	for (int j = 0; j < nodes.rows; ++j)
	{
		for (int i = 0; i < nodes.cols; ++i) EXPECT_TRUE(everywhere.contains({ nodes.col(i), nodes.row(j) }));
	}

	// A view over the edge of the grid ends with the grid; a view beside it has nothing.
	const ThinnedNodes corner = thinnedNodes(grid, 4, GeoBounds{ .minLon = 120, .minLat = 30, .maxLon = 126.05, .maxLat = 34.05 });
	EXPECT_EQ(corner.firstCol, 0);
	EXPECT_EQ(corner.firstRow, 0);
	EXPECT_EQ(corner.col(corner.cols - 1), 8);
	EXPECT_EQ(corner.row(corner.rows - 1), 8);
	EXPECT_TRUE(thinnedNodes(grid, 4, GeoBounds{ .minLon = 130, .minLat = 34, .maxLon = 131, .maxLat = 35 }).empty());
	EXPECT_TRUE(thinnedNodes(grid, 4, GeoBounds{ .minLon = 126, .minLat = 40, .maxLon = 127, .maxLat = 41 }).empty());
}

TEST(ArrowThinning, ThinArrowsLeavesNoOverlapAtAnyZoom)
{
	// Zooming out from 20 mm between nodes to 0.02 mm: the longest arrow (26 mm) never exceeds
	// half the diagonal between the nodes drawn, and no more nodes are dropped than needed.
	int previous = 0;
	for (double scale = 2000; scale >= 2; scale /= 1.7)
	{
		const ThinnedNodes nodes = thinArrows(grid, flatChart(scale), maxArrowLength());
		const double diagonal = nodeDiagonal(grid, flatChart(scale));
		ASSERT_FALSE(nodes.empty());
		EXPECT_LE(maxArrowLength() / (nodes.factor * diagonal), MaxArrowToSpacingRatio) << scale;
		EXPECT_GE(nodes.factor, previous) << "coarser as the chart is zoomed out";
		previous = nodes.factor;
	}
	EXPECT_GT(previous, 100);

	// Zoomed in far enough, every node; with a view, only the nodes around it.
	EXPECT_EQ(thinArrows(grid, flatChart(5000), maxArrowLength()).factor, 1);
	const GeoBounds view{ .minLon = 126.30, .minLat = 34.20, .maxLon = 126.40, .maxLat = 34.30 };
	const ThinnedNodes seen = thinArrows(grid, flatChart(1000), maxArrowLength(), view);
	EXPECT_EQ(seen.factor, 4);   // D = 14.14 mm: 1 + fix(26 / 7.07)
	EXPECT_EQ(seen.firstCol, 28);
	EXPECT_EQ(seen.col(seen.cols - 1), 44);
	EXPECT_EQ(seen.firstRow, 16);
	EXPECT_EQ(seen.row(seen.rows - 1), 32);

	// A slower field has shorter arrows and keeps more of them (L_smax of the field shown).
	EXPECT_EQ(thinArrows(grid, flatChart(1000), arrowLength(1.5)).factor, 1);   // 4 mm against 14.14 mm
}

TEST(ArrowThinning, WorksOnAUtmGrid)
{
	// 100 m nodes in UTM zone 52N, on a chart of 1 mm to 100 m of longitude at 35 N.
	const RegularGrid utm{ .originLon = 300000, .originLat = 3870000, .spacingLon = 100, .spacingLat = 100, .cols = 200, .rows = 200,
		.utm = UtmZone{ .zone = 52, .south = false } };
	const double metresPerDegree = 111320.0;
	const ToScreen chart = [=](const GeoPosition& position) {
		return ScreenPoint{ position.lon * metresPerDegree * std::cos(35 * 3.14159265358979 / 180) / 100, -position.lat * metresPerDegree / 100 };
	};
	const double diagonal = nodeDiagonal(utm, chart);
	EXPECT_NEAR(diagonal, std::sqrt(2.0), 0.02);
	const ThinnedNodes nodes = thinArrows(utm, chart, maxArrowLength());
	EXPECT_EQ(nodes.factor, 1 + static_cast<int>(26 / (0.5 * diagonal)));
	EXPECT_EQ(nodes.factor, 37);
	EXPECT_EQ(nodes.cols, 6);   // 0, 37, ..., 185

	// A view around the middle of the grid keeps the nodes around it.
	const GeoPosition middle = gridPosition(utm, 100, 100);
	const ThinnedNodes seen = thinnedNodes(utm, 10, GeoBounds{ .minLon = middle.lon - 0.005, .minLat = middle.lat - 0.005,
		.maxLon = middle.lon + 0.005, .maxLat = middle.lat + 0.005 });
	ASSERT_FALSE(seen.empty());
	EXPECT_LE(seen.firstCol, 90);
	EXPECT_GE(seen.col(seen.cols - 1), 110);
	EXPECT_LT(seen.cols, 8);
}

TEST(ArrowThinning, SymbolScaleAndRotationFollowTheLookupEntry)
{
	// S-111 PC 2.0.0: the slow bands at a fixed 0.4, the middle ones at speed / 5, turned by the direction.
	const CoverageSymbol slow{ .reference = "SCAROW01", .rotationAttribute = "surfaceCurrentDirection", .defaultScaleFactor = 0.4 };
	EXPECT_DOUBLE_EQ(symbolScale(slow, 0.3), 0.4);
	EXPECT_DOUBLE_EQ(symbolScale(slow, std::nullopt), 0.4);
	EXPECT_DOUBLE_EQ(symbolRotation(slow, 135.0), 135.0);

	const CoverageSymbol middle{ .reference = "SCAROW05", .rotationAttribute = "surfaceCurrentDirection",
		.scaleAttribute = "surfaceCurrentSpeed", .scaleFactor = 0.2 };
	EXPECT_DOUBLE_EQ(symbolScale(middle, 4.0), 0.8);
	EXPECT_DOUBLE_EQ(symbolScale(middle, std::nullopt), 1.0) << "the default without a value";
	// The same lengths as equation 9.1 gives.
	EXPECT_DOUBLE_EQ(symbolScale(middle, 4.0), arrowScaleFactor(4.0));
	EXPECT_DOUBLE_EQ(symbolScale(slow, 0.3), arrowScaleFactor(0.3));

	const CoverageSymbol fixed{ .reference = "X", .rotation = 90 };
	EXPECT_DOUBLE_EQ(symbolRotation(fixed, 10.0), 90.0);
	const CoverageSymbol reversed{ .reference = "X", .rotationAttribute = "d", .rotationFactor = -1 };
	EXPECT_DOUBLE_EQ(symbolRotation(reversed, 30.0), -30.0);
}
