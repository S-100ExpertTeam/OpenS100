#include <gtest/gtest.h>

#include "Coverage/CoverageInstructions.h"
#include "Coverage/CoverageRaster.h"

#include <numeric>
#include <vector>

// Coverage backport CV-3 (Documents/TechDocs/2026-11-11-coverage-backport-plan.md): coverage
// instructions as the S-102 portrayal catalogue emits them, lookup colours and the Mercator
// raster, with the cases of opens100-web `packages/render/test/coverage-raster.test.ts`.
using namespace s100::coverage;

namespace
{
	// Node index as the value, and as the colour: the pixel says which node it shows.
	const CellColor nodeColor = [](double value) -> std::optional<Rgba> {
		const auto node = static_cast<std::uint32_t>(value);
		return Rgba{ static_cast<std::uint8_t>(node & 255), static_cast<std::uint8_t>((node >> 8) & 255), static_cast<std::uint8_t>((node >> 16) & 255), 255 };
	};

	int nodeOf(std::uint32_t pixel)
	{
		return (pixel >> 24) == 0 ? -1 : static_cast<int>(pixel & 0xFFFFFF);
	}

	// What BathymetryCoverage.lua of the S-102 PC 3.0.0 emits with SafetyContour 30.
	constexpr const char* twoShades = "ViewingGroup:13030;DrawingPriority:3;DisplayPlane:UnderRadar;"
		"CoverageColor:DEPIT,0;LookupEntry:Intertidal,,0,ltSemiInterval;"
		"CoverageColor:DEPVS,0;LookupEntry:Shallow Water,0,30,geLtInterval;"
		"CoverageColor:DEPDW,0;LookupEntry:Deep Water,30,,geSemiInterval;CoverageFill:depth";
}

TEST(CoverageInstructions, ReadsTheS102LookupTable)
{
	const auto instructions = parseCoverageInstructions(twoShades);
	ASSERT_EQ(instructions.size(), 1u);
	const CoverageInstruction& fill = instructions[0];
	EXPECT_EQ(fill.viewingGroups, std::vector<int>{ 13030 });
	EXPECT_EQ(fill.drawingPriority, 3);
	EXPECT_EQ(fill.displayPlane, "UnderRadar");
	EXPECT_EQ(fill.attributeCode, "depth");
	ASSERT_EQ(fill.lookup.size(), 3u);
	EXPECT_EQ(fill.lookup[0].label, "Intertidal");
	EXPECT_EQ(fill.lookup[0].lower, std::nullopt);
	EXPECT_EQ(fill.lookup[0].upper, 0.0);
	EXPECT_EQ(fill.lookup[0].closure, IntervalClosure::LtSemiInterval);
	EXPECT_EQ(fill.lookup[0].color->startToken, "DEPIT");
	EXPECT_EQ(fill.lookup[1].lower, 0.0);
	EXPECT_EQ(fill.lookup[1].upper, 30.0);
	EXPECT_EQ(fill.lookup[2].color->startToken, "DEPDW");
	EXPECT_EQ(fill.lookup[2].upper, std::nullopt);
}

TEST(CoverageInstructions, FourShadesGiveFiveEntries)
{
	const auto instructions = parseCoverageInstructions("ViewingGroup:13030;DrawingPriority:3;DisplayPlane:UnderRadar;"
		"CoverageColor:DEPIT,0;LookupEntry:Intertidal,,0,ltSemiInterval;"
		"CoverageColor:DEPVS,0;LookupEntry:Shallow Water,0,2,geLtInterval;"
		"CoverageColor:DEPMS,0;LookupEntry:Medium-Shallow Water,2,30,geLtInterval;"
		"CoverageColor:DEPMD,0;LookupEntry:Medium-Deep Water,30,30,geLtInterval;"
		"CoverageColor:DEPDW,0;LookupEntry:Deep Water,30,,geSemiInterval;CoverageFill:depth");
	ASSERT_EQ(instructions.size(), 1u);
	ASSERT_EQ(instructions[0].lookup.size(), 5u);
	// [30, 30) holds nothing: 30 m is deep water.
	EXPECT_EQ(lookupCoverageEntry(instructions[0].lookup, 30)->index, 4u);
	EXPECT_EQ(lookupCoverageEntry(instructions[0].lookup, 1.5)->index, 1u);
}

TEST(CoverageInstructions, ColoursValuesByTheirEntryAndLeavesTheRestEmpty)
{
	auto instructions = parseCoverageInstructions("CoverageColor:DEPIT,0;LookupEntry:a,,0,ltSemiInterval;"
		"CoverageColor:DEPVS,0.5;LookupEntry:b,0,30,geLtInterval;CoverageFill:depth");
	const CellColor colorOf = lookupCellColor(instructions[0].lookup, [](std::string_view token) {
		return token == "DEPIT" ? Rgb{ 88, 175, 156 } : Rgb{ 97, 183, 255 };
	});
	EXPECT_EQ(colorOf(-1), (Rgba{ 88, 175, 156, 255 }));
	EXPECT_EQ(colorOf(10), (Rgba{ 97, 183, 255, 128 }));
	EXPECT_EQ(colorOf(40), std::nullopt);
}

TEST(CoverageRaster, LaysAGeographicGridOutInMercatorNorthRowFirst)
{
	const RegularGrid grid{ .originLon = 128, .originLat = 34, .spacingLon = 0.5, .spacingLat = 0.25, .cols = 3, .rows = 2 };
	const std::vector<float> values{ 0, 1, 2, 3, 4, 5 };
	const CoverageRaster raster(grid, values, -1, nodeColor);
	std::vector<int> nodes;
	for (const std::uint32_t pixel : raster.pixels()) nodes.push_back(nodeOf(pixel));
	EXPECT_EQ(nodes, (std::vector<int>{ 3, 4, 5, 0, 1, 2 }));
	EXPECT_EQ(raster.extent().west, 127.75);
	EXPECT_EQ(raster.extent().east, 129.25);
	EXPECT_GT(raster.extent().north, raster.extent().south);
	EXPECT_NEAR(inverseMercatorY(mercatorY(34.25)), 34.25, 1e-12);
}

// The OpenS100 viewer draws an equirectangular chart: the image's rows must be spaced evenly in
// latitude, each image row edge on a cell edge, where the safety contour and the S-102 boundary
// run. Spaced in Mercator, the rows between the corners drift from them (by about 0.4 of a cell in
// the middle of the KOR S-102, 0.5 degrees tall at 35 N).
TEST(CoverageRaster, LaysAGeographicGridOutRowForRowOnAGeographicChart)
{
	const RegularGrid grid{ .originLon = 129.0005, .originLat = 35.0005, .spacingLon = 0.001, .spacingLat = 0.001, .cols = 500, .rows = 500 };
	std::vector<float> values(grid.nodeCount());
	std::iota(values.begin(), values.end(), 0.0f);
	const CoverageRaster raster(grid, values, -1, nodeColor, RasterProjection::Geographic);
	ASSERT_EQ(raster.width(), grid.cols);
	ASSERT_EQ(raster.height(), grid.rows);
	const auto& extent = raster.extent();
	EXPECT_NEAR(raster.latitude(extent.north), 35.5, 1e-9);
	EXPECT_NEAR(raster.latitude(extent.south), 35.0, 1e-9);
	for (int y = 0; y <= raster.height(); ++y)
	{
		// The edge between image rows y - 1 and y, placed linearly between the corners.
		const double edge = extent.north - y * (extent.north - extent.south) / raster.height();
		const double cellEdge = grid.originLat + (grid.rows - y - 0.5) * grid.spacingLat;
		ASSERT_NEAR(raster.latitude(edge), cellEdge, 1e-9) << "row edge " << y;
	}
	// Image row y shows grid row rows - 1 - y.
	for (int y : { 0, 1, 249, 499 })
	{
		EXPECT_EQ(nodeOf(raster.pixels()[static_cast<std::size_t>(y) * raster.width()]), (grid.rows - 1 - y) * grid.cols) << y;
	}

	// The same grid laid out for Web Mercator: its row edges are off the cell edges in latitude
	// by up to some tenths of a cell, which is what the viewer showed before.
	const CoverageRaster mercator(grid, values, -1, nodeColor);
	double worst = 0;
	for (int y = 0; y <= mercator.height(); ++y)
	{
		const double edge = mercator.extent().north - y * (mercator.extent().north - mercator.extent().south) / mercator.height();
		const double row = (35.5 - inverseMercatorY(edge)) / grid.spacingLat;
		worst = (std::max)(worst, std::abs(row - std::round(row)));
	}
	EXPECT_GT(worst, 0.1) << "Mercator rows placed linearly in latitude miss the cell edges";
}

TEST(CoverageRaster, WarpsAUtmGridIntoMercator)
{
	// Far from the central meridian (135 W) so the grid is plainly turned on the chart.
	const RegularGrid grid{ .originLon = 690000, .originLat = 6050000, .spacingLon = 16, .spacingLat = 16, .cols = 300, .rows = 200,
		.utm = UtmZone{ 8, false } };
	std::vector<float> values(grid.nodeCount());
	std::iota(values.begin(), values.end(), 0.0f);
	const CoverageRaster raster(grid, values, -1, nodeColor);
	const auto& extent = raster.extent();
	const double pixels = static_cast<double>(raster.width()) * raster.height();
	EXPECT_GE(pixels, static_cast<double>(grid.nodeCount()));
	EXPECT_LE(pixels, 1.25 * static_cast<double>(grid.nodeCount()) * 1.2);

	// Each pixel shows the cell its centre falls in, or nothing outside the turned grid.
	int checked = 0;
	int mismatched = 0;
	for (int y = 0; y < raster.height(); y += 7)
	{
		for (int x = 0; x < raster.width(); x += 11)
		{
			const GeoPosition centre{ extent.west + (x + 0.5) * (extent.east - extent.west) / raster.width(),
				inverseMercatorY(extent.north - (y + 0.5) * (extent.north - extent.south) / raster.height()) };
			const auto cell = gridCellAt(grid, centre);
			const int expected = cell ? cell->row * grid.cols + cell->col : -1;
			++checked;
			if (nodeOf(raster.pixels()[static_cast<std::size_t>(y) * raster.width() + x]) != expected) ++mismatched;
		}
	}
	EXPECT_LT(mismatched, checked / 100 + 1);
}
