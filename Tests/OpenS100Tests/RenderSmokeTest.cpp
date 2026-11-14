// Off-screen render smoke test.
//
// Draws an S-101 cell through the same path the viewer uses (CGISLibraryApp::Draw into an HDC),
// but into a memory bitmap, so it needs no window and no interactive desktop. The picture is
// saved as PNG for a human (or an agent) to look at:
//
//   set S100_RENDER_OUT=<directory>     default: the per-run temp directory of the tests
//
// The assertion is deliberately coarse (the chart is not blank); pixel-exact golden images
// come with the display-list work in Phase 3.
#include <gtest/gtest.h>

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

#include "TestPaths.h"
#include "GISLibrary.h"
#include "PortrayalRenderCache.h"
#include "S100ExchangeCatalogue.h"
#include "CoverageDataset.h"
#include "S10XGML.h"
#include "SystemDatabaseSession.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"

namespace
{
	struct GdiplusSession
	{
		ULONG_PTR token = 0;
		GdiplusSession()
		{
			Gdiplus::GdiplusStartupInput input;
			Gdiplus::GdiplusStartup(&token, &input, nullptr);
		}
		~GdiplusSession() { Gdiplus::GdiplusShutdown(token); }
	};

	// 32-bit top-down DIB selected into a memory DC.
	class MemoryCanvas
	{
	public:
		MemoryCanvas(int width, int height) : m_width(width), m_height(height)
		{
			BITMAPINFO info{};
			info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			info.bmiHeader.biWidth = width;
			info.bmiHeader.biHeight = -height;
			info.bmiHeader.biPlanes = 1;
			info.bmiHeader.biBitCount = 32;
			info.bmiHeader.biCompression = BI_RGB;
			m_dc = ::CreateCompatibleDC(nullptr);
			m_bitmap = ::CreateDIBSection(m_dc, &info, DIB_RGB_COLORS, reinterpret_cast<void**>(&m_bits), nullptr, 0);
			m_old = ::SelectObject(m_dc, m_bitmap);
			const RECT all{ 0, 0, width, height };
			::FillRect(m_dc, &all, static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH)));
		}
		~MemoryCanvas()
		{
			::SelectObject(m_dc, m_old);
			::DeleteObject(m_bitmap);
			::DeleteDC(m_dc);
		}
		HDC& dc() { return m_dc; }
		bool valid() const { return m_dc && m_bitmap && m_bits; }

		// Pixels that are not plain white, and how many distinct colours appear (capped).
		void Measure(std::size_t& nonWhite, std::size_t& distinctColours) const
		{
			::GdiFlush();
			std::vector<std::uint32_t> seen;
			nonWhite = 0;
			for (int i = 0; i < m_width * m_height; ++i)
			{
				const std::uint32_t rgb = m_bits[i] & 0x00FFFFFF;
				if (rgb != 0x00FFFFFF)
				{
					++nonWhite;
				}
				if (seen.size() < 4096 && std::find(seen.begin(), seen.end(), rgb) == seen.end())
				{
					seen.push_back(rgb);
				}
			}
			distinctColours = seen.size();
		}

		// Pixels that differ from the same pixel of `other` (of the same size).
		std::size_t DifferingPixels(const MemoryCanvas& other) const
		{
			::GdiFlush();
			std::size_t count = 0;
			for (int i = 0; i < m_width * m_height; ++i)
			{
				if ((m_bits[i] & 0x00FFFFFF) != (other.m_bits[i] & 0x00FFFFFF)) ++count;
			}
			return count;
		}

		bool SavePng(const std::filesystem::path& path) const
		{
			::GdiFlush();
			Gdiplus::Bitmap image(m_width, m_height, m_width * 4, PixelFormat32bppRGB, reinterpret_cast<BYTE*>(m_bits));
			// image/png encoder
			const CLSID png{ 0x557CF406, 0x1A04, 0x11D3, { 0x9A, 0x73, 0x00, 0x00, 0xF8, 0x1E, 0xF3, 0x2E } };
			return image.Save(path.wstring().c_str(), &png, nullptr) == Gdiplus::Ok;
		}

	private:
		int m_width;
		int m_height;
		HDC m_dc = nullptr;
		HBITMAP m_bitmap = nullptr;
		HGDIOBJ m_old = nullptr;
		std::uint32_t* m_bits = nullptr;
	};

	std::filesystem::path OutputDirectory()
	{
		char* value = nullptr;
		std::size_t length = 0;
		if (_dupenv_s(&value, &length, "S100_RENDER_OUT") == 0 && value)
		{
			std::filesystem::path dir(value);
			free(value);
			std::error_code ec;
			std::filesystem::create_directories(dir, ec);
			return dir;
		}
		return testpaths::tempDir();
	}
}

namespace
{
	constexpr int kWidth = 1600;
	constexpr int kHeight = 1000;

	// Opens `cell` in a fresh library instance, draws it fitted to the canvas and saves a PNG.
	void RenderCell(const std::filesystem::path& cell, std::size_t& nonWhite, std::size_t& colours)
	{
		using Clock = std::chrono::steady_clock;
		const auto ms = [](Clock::time_point a, Clock::time_point b)
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count();
		};

		GdiplusSession gdiplus;
		CGISLibraryApp gis;

		const auto t0 = Clock::now();
		auto cm = gis.GetCatalogManager();
		ASSERT_NE(cm->addFC((testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml").string()), nullptr);
		auto pc = cm->addPC((testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml").wstring());
		ASSERT_NE(pc, nullptr);
		GISLibrary::PortrayalRender::Prepare(pc, gis.GetD2D1Resources());
		const auto t1 = Clock::now();

		const RECT view{ 0, 0, kWidth, kHeight };
		gis.SetScreen(view);

		// Opening a layer parses the dataset and runs the portrayal rules.
		ASSERT_TRUE(gis.AddLayer(cell.wstring())) << cell.string();
		gis.SetViewMBR(view);
		const auto t2 = Clock::now();

		MemoryCanvas canvas(kWidth, kHeight);
		ASSERT_TRUE(canvas.valid());
		gis.Draw(canvas.dc());
		::GdiFlush();
		const auto t3 = Clock::now();

		// Further frames of the same view: these must not get slower.
		std::string frames;
		for (int i = 0; i < 4; ++i)
		{
			const auto a = Clock::now();
			gis.Draw(canvas.dc());
			::GdiFlush();
			frames += " " + std::to_string(ms(a, Clock::now()));
		}

		const auto png = OutputDirectory() / (cell.stem().string() + ".png");
		EXPECT_TRUE(canvas.SavePng(png)) << png.string();
		canvas.Measure(nonWhite, colours);
		std::cout << "[ render   ] " << png.string() << "  non-white pixels: " << nonWhite
			<< ", distinct colours: " << colours << std::endl;
		std::cout << "[ timing   ] " << cell.stem().string() << "  catalogues " << ms(t0, t1) << " ms, open+portrayal "
			<< ms(t1, t2) << " ms, first draw " << ms(t2, t3) << " ms, next frames" << frames << " ms" << std::endl;
	}
}

TEST(RenderSmoke, DrawsKoreanS101CellOffscreen)
{
	const auto cell = testpaths::korS101Cell();
	if (!std::filesystem::exists(cell))
	{
		GTEST_SKIP() << "reference cell not available: " << cell.string();
	}

	std::size_t nonWhite = 0;
	std::size_t colours = 0;
	RenderCell(cell, nonWhite, colours);
	if (HasFatalFailure())
	{
		return;
	}
	EXPECT_GT(nonWhite, static_cast<std::size_t>(kWidth) * kHeight / 50) << "the chart is (almost) blank";
	EXPECT_GT(colours, 8u) << "expected more than a handful of colours in a portrayed chart";
}

// Opt-in: render the cells named in S100_RENDER_CELLS (';'-separated paths) for visual review.
TEST(RenderSmoke, DrawsCellsFromEnvironment)
{
	char* value = nullptr;
	std::size_t length = 0;
	if (_dupenv_s(&value, &length, "S100_RENDER_CELLS") != 0 || !value)
	{
		GTEST_SKIP() << "S100_RENDER_CELLS is not set";
	}
	const std::string list(value);
	free(value);

	std::size_t start = 0;
	while (start <= list.size())
	{
		const auto end = list.find(';', start);
		const std::string item = list.substr(start, end == std::string::npos ? std::string::npos : end - start);
		start = end == std::string::npos ? list.size() + 1 : end + 1;
		if (item.empty())
		{
			continue;
		}
		const std::filesystem::path cell(item);
		ASSERT_TRUE(std::filesystem::exists(cell)) << item;
		std::size_t nonWhite = 0;
		std::size_t colours = 0;
		RenderCell(cell, nonWhite, colours);
		EXPECT_GT(nonWhite, 0u) << item;
	}
}

// System Database work item SDB-5: an exchange set opened without installing it loads only the
// cells S-98 E-1.3 selects for the view, through the same selector as the System Database.
TEST(RenderSmoke, ExchangeSetLoadsOnlyTheSelectedCells)
{
	const auto catalog = testpaths::korS101Catalog();
	if (!std::filesystem::exists(catalog))
	{
		GTEST_SKIP() << "reference exchange set not available: " << catalog.string();
	}

	GdiplusSession gdiplus;
	CGISLibraryApp gis;
	auto cm = gis.GetCatalogManager();
	ASSERT_NE(cm->addFC((testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml").string()), nullptr);
	auto pc = cm->addPC((testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml").wstring());
	ASSERT_NE(pc, nullptr);
	GISLibrary::PortrayalRender::Prepare(pc, gis.GetD2D1Resources());

	const RECT view{ 0, 0, kWidth, kHeight };
	gis.SetScreen(view);
	ASSERT_TRUE(gis.AddLayer(catalog.wstring()));

	S100ExchangeCatalogue* exchangeSet = nullptr;
	for (Layer* layer : gis.GetLayerManager()->layers)
	{
		if (auto* candidate = dynamic_cast<S100ExchangeCatalogue*>(layer->m_spatialObject))
		{
			exchangeSet = candidate;
		}
	}
	ASSERT_NE(exchangeSet, nullptr);
	EXPECT_EQ(exchangeSet->m_vecInventory.size(), 808u);

	// Busan harbour at about 1:50,000, then the whole south coast at about 1:1,500,000.
	const auto drawAt = [&](double west, double south, double east, double north, const char* name) {
		MBR area(west, south, east, north);
		projection(area);
		gis.SetMap(area);
		MemoryCanvas canvas(kWidth, kHeight);
		const auto start = std::chrono::steady_clock::now();
		gis.Draw(canvas.dc());
		const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
		std::size_t nonWhite = 0;
		std::size_t colours = 0;
		canvas.Measure(nonWhite, colours);
		const auto png = OutputDirectory() / (std::string("exchange-set-") + name + ".png");
		EXPECT_TRUE(canvas.SavePng(png));
		const std::size_t loaded = exchangeSet->m_pLayerManager->layers.size();
		std::cout << "[ selection ] " << name << " 1:" << gis.GetScaler()->GetCurrentScale() << " loaded " << loaded
			<< " of 808 cells, first draw " << ms << " ms, " << png.string() << std::endl;
		EXPECT_GT(loaded, 0u) << name;
		EXPECT_LT(loaded, 100u) << name;
		EXPECT_GT(nonWhite, static_cast<std::size_t>(kWidth) * kHeight / 50) << name << ": the chart is (almost) blank";
	};
	drawAt(128.95, 35.02, 129.15, 35.14, "busan");
	drawAt(125.5, 33.5, 130.5, 36.0, "south-coast");

	// SDB-6: the Busan cells left the selection for the cache; going back parses nothing again.
	const auto* layerSet = exchangeSet->GetLayerSet();
	ASSERT_NE(layerSet, nullptr);
	EXPECT_GT(layerSet->cachedCount(), 0u);
	const std::size_t opened = layerSet->openCount();
	drawAt(128.95, 35.02, 129.15, 35.14, "busan-again");
	EXPECT_EQ(layerSet->openCount(), opened);
}

// System Database work item SDB-7: datasets installed once are drawn after a restart by S-98 E-1.3
// selection: nothing is opened at start-up, a view opens only the cells it needs.
TEST(RenderSmoke, SystemDatabaseChartLoadsBySelectionAfterRestart)
{
	const auto cells = testpaths::tdsRoot() / L"KOR_TDS" / L"S100_ROOT" / L"S-101" / L"DATASET_FILES";
	if (!std::filesystem::exists(cells))
	{
		GTEST_SKIP() << "reference exchange set not available: " << cells.string();
	}
	const auto root = testpaths::tempDir() / L"SystemDatabaseChart";
	std::filesystem::remove_all(root);

	// Every cell over Busan, from the overview to the harbour plans.
	const char* busan[] = { "101KR00659C81", "101KR00659C82", "101KR005G3B33", "101KR004G3B30", "101KR003G3B00", "101KR002G3000", "101KR001G0000" };
	{
		GISLibrary::SystemDatabaseSession session(root);
		ASSERT_EQ(session.importer().importFeatureCatalogue(testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml").outcome,
			s100::sdb::ImportOutcome::Added);
		ASSERT_EQ(session.importer().importPortrayalCatalogue(testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal").outcome,
			s100::sdb::ImportOutcome::Added);
		for (const char* name : busan)
		{
			const auto file = cells / name / (std::string(name) + ".000");
			ASSERT_EQ(session.importer().importDataset(file).outcome, s100::sdb::ImportOutcome::Added) << name;
		}
	}

	// "Restart": a new library instance over the same System Database.
	GdiplusSession gdiplus;
	CGISLibraryApp gis;
	auto* catalogues = gis.GetCatalogManager();
	catalogues->onPortrayalLoaded = [&gis](PortrayalCatalogue* pc) { GISLibrary::PortrayalRender::Prepare(pc, gis.GetD2D1Resources()); };
	const RECT view{ 0, 0, kWidth, kHeight };
	gis.SetScreen(view);

	GISLibrary::SystemDatabaseSession session(root);
	session.attach(*gis.GetLayerManager(), *catalogues);
	auto* chart = session.chart();
	ASSERT_NE(chart, nullptr);
	EXPECT_GE(chart->GetSelector().coverages().size(), 7u);   // some cells have more than one
	EXPECT_EQ(chart->GetLayerSet().openCount(), 0u) << "nothing is opened at start-up";
	EXPECT_TRUE(catalogues->FCs.empty()) << "nor is a catalogue read";

	const auto drawAt = [&](double west, double south, double east, double north, const char* name) {
		MBR area(west, south, east, north);
		projection(area);
		gis.SetMap(area);
		MemoryCanvas canvas(kWidth, kHeight);
		gis.Draw(canvas.dc());
		std::size_t nonWhite = 0;
		std::size_t colours = 0;
		canvas.Measure(nonWhite, colours);
		const auto png = OutputDirectory() / (std::string("sdb-chart-") + name + ".png");
		EXPECT_TRUE(canvas.SavePng(png));
		std::cout << "[ sdb chart ] " << name << " 1:" << gis.GetScaler()->GetCurrentScale() << " shown "
			<< chart->GetLayerSet().shownCount() << ", cached " << chart->GetLayerSet().cachedCount() << ", opened so far "
			<< chart->GetLayerSet().openCount() << ", " << png.string() << std::endl;
		EXPECT_GT(nonWhite, static_cast<std::size_t>(kWidth) * kHeight / 50) << name << ": the chart is (almost) blank";
	};

	drawAt(129.00, 35.05, 129.10, 35.11, "busan-harbour");
	const std::size_t harbour = chart->GetLayerSet().shownCount();
	EXPECT_GT(harbour, 0u);
	EXPECT_LT(harbour, 7u);
	EXPECT_EQ(catalogues->FCs.size(), 1u);

	drawAt(126.0, 33.0, 131.0, 37.0, "south-coast");
	EXPECT_GT(chart->GetLayerSet().cachedCount(), 0u) << "the harbour cells left the view for the cache";

	const std::size_t opened = chart->GetLayerSet().openCount();
	drawAt(129.00, 35.05, 129.10, 35.11, "busan-harbour-again");
	EXPECT_EQ(chart->GetLayerSet().openCount(), opened) << "coming back parses nothing again";

	session.detach();
	EXPECT_TRUE(gis.GetLayerManager()->layers.empty());
}

// A gridded dataset (S-102) has no display scales: the System Database chart shows it whenever its
// bounding box meets the view, and draws the grid itself.
// Coverage backport CV-3 (Documents/TechDocs/2026-11-11-coverage-backport-plan.md): the S-102
// portrayal catalogue's rules colour the grid, drawn as one image.
TEST(RenderSmoke, SystemDatabaseChartDrawsS102Grid)
{
	const auto grid = testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-102" / L"S-102" / L"102KR00T4G3B3000026.h5";
	if (!std::filesystem::exists(grid))
	{
		GTEST_SKIP() << "S-102 test grid not available: " << grid.string();
	}
	const auto root = testpaths::tempDir() / L"SystemDatabaseChartS102";
	std::filesystem::remove_all(root);
	{
		GISLibrary::SystemDatabaseSession session(root);
		ASSERT_EQ(session.importer().importFeatureCatalogue(testpaths::repoRoot() / "ProgramData" / "FC" / "S-102_FC.xml").outcome,
			s100::sdb::ImportOutcome::Added);
		// The portrayal catalogue as IHO delivers it.
		const auto pcZip = testpaths::tdsRoot() / L"FC_PC" / L"S-102" / L"unzipped" / L"S-102" / L"CATALOGUES" / L"102_Portrayal_Catalogue_3.0.0.zip";
		if (!std::filesystem::exists(pcZip))
		{
			GTEST_SKIP() << "S-102 portrayal catalogue not available: " << pcZip.string();
		}
		ASSERT_EQ(session.importer().importPortrayalCatalogue(pcZip).outcome, s100::sdb::ImportOutcome::Added);
		ASSERT_EQ(session.importer().importDataset(grid).outcome, s100::sdb::ImportOutcome::Added);
	}

	GdiplusSession gdiplus;
	CGISLibraryApp gis;
	auto* catalogues = gis.GetCatalogManager();
	catalogues->onPortrayalLoaded = [&gis](PortrayalCatalogue* pc) { GISLibrary::PortrayalRender::Prepare(pc, gis.GetD2D1Resources()); };
	const RECT view{ 0, 0, kWidth, kHeight };
	gis.SetScreen(view);

	GISLibrary::SystemDatabaseSession session(root);
	session.attach(*gis.GetLayerManager(), *catalogues);
	auto* chart = session.chart();
	ASSERT_NE(chart, nullptr);
	const auto record = session.database().datasets().find("S-102", "102KR00T4G3B3000026");
	ASSERT_TRUE(record && record->bounds);

	MBR area(record->bounds->minX, record->bounds->minY, record->bounds->maxX, record->bounds->maxY);
	projection(area);
	gis.SetMap(area);
	MemoryCanvas canvas(kWidth, kHeight);
	gis.Draw(canvas.dc());
	std::size_t nonWhite = 0;
	std::size_t colours = 0;
	canvas.Measure(nonWhite, colours);
	const auto png = OutputDirectory() / "sdb-chart-s102.png";
	EXPECT_TRUE(canvas.SavePng(png));
	std::cout << "[ sdb chart ] S-102 1:" << gis.GetScaler()->GetCurrentScale() << " shown " << chart->GetLayerSet().shownCount()
		<< ", opened " << chart->GetLayerSet().openCount() << ", non-white " << nonWhite << ", colours " << colours << ", "
		<< png.string() << std::endl;
	EXPECT_TRUE(chart->GetLayerSet().isShown(record->id)) << "the grid is in view";
	if (auto* inner = chart->GetInnerLayers(); inner && inner->LayerCount() == 1)
	{
		// Portrayed by the S-102 catalogue: one CoverageFill of depth with its lookup table.
		auto* coverage = dynamic_cast<GISLibrary::CoverageDataset*>(inner->GetLayer(0)->GetSpatialObject());
		ASSERT_NE(coverage, nullptr) << "S-102 opens as a coverage portrayed by its catalogue";
		ASSERT_EQ(coverage->GetInstructions().size(), 1u);
		EXPECT_EQ(coverage->GetInstructions()[0].attributeCode, "depth");
		EXPECT_GE(coverage->GetInstructions()[0].lookup.size(), 3u);
	}
	EXPECT_GE(colours, 4u) << "white and the three depth classes of the two-shade table (DEPIT, DEPVS, DEPDW)";
}

// The same S-102 grid opened directly as a layer (not through the System Database): the reference
// for what the grid renderer draws.
TEST(RenderSmoke, DrawsS102GridAsLayer)
{
	const auto grid = testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-102" / L"S-102" / L"102KR00T4G3B3000026.h5";
	if (!std::filesystem::exists(grid))
	{
		GTEST_SKIP() << "S-102 test grid not available: " << grid.string();
	}
	GdiplusSession gdiplus;
	CGISLibraryApp gis;
	auto* catalogues = gis.GetCatalogManager();
	ASSERT_NE(catalogues->addFC((testpaths::repoRoot() / "ProgramData" / "FC" / "S-102_FC.xml").string()), nullptr);
	// The S-102 PC 3.0.0 opens100-web ships, as a folder.
	const auto pc = testpaths::webDataRoot().parent_path() / L"pc" / L"S102_Portrayal" / L"portrayal_catalogue.xml";
	if (!std::filesystem::exists(pc))
	{
		GTEST_SKIP() << "S-102 portrayal catalogue not available: " << pc.string();
	}
	ASSERT_NE(catalogues->addPC(pc.wstring()), nullptr);
	const RECT view{ 0, 0, kWidth, kHeight };
	gis.SetScreen(view);
	ASSERT_TRUE(gis.AddLayer(grid.wstring()));
	gis.SetViewMBR(view);
	MemoryCanvas canvas(kWidth, kHeight);
	gis.Draw(canvas.dc());
	std::size_t nonWhite = 0;
	std::size_t colours = 0;
	canvas.Measure(nonWhite, colours);
	const auto png = OutputDirectory() / "s102-layer.png";
	EXPECT_TRUE(canvas.SavePng(png));
	std::cout << "[ s102 layer ] onIC " << gis.GetLayerManager()->onIC << ", non-white " << nonWhite << ", colours " << colours
		<< ", " << png.string() << std::endl;
	EXPECT_GE(colours, 4u) << "white and the three depth classes of the two-shade table";
}

// S-111 surface currents: the arrows of the S-111 portrayal catalogue (XSLT rules, SCAROW symbols)
// at the grid points of the record in force at the display time, thinned as the chart is zoomed
// out (S-111 Edition 2.0.0 clauses 9.3.2 and 9.4).
TEST(RenderSmoke, DrawsS111ArrowsThinnedByZoom)
{
	const auto s111 = testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-111" / L"S-111" / L"111KR00KR4_MOHID2K_G3B30_20260330_F36.h5";
	if (!std::filesystem::exists(s111))
	{
		GTEST_SKIP() << "KOR S-111 not available: " << s111.string();
	}
	using GISLibrary::CoverageDataset;
	const CoverageDataset::DisplaySettings original = CoverageDataset::GetDisplaySettings();

	GdiplusSession gdiplus;
	CGISLibraryApp gis;
	auto* catalogues = gis.GetCatalogManager();
	ASSERT_NE(catalogues->addFC((testpaths::repoRoot() / "ProgramData" / "FC" / "S-111_FC.xml").string()), nullptr);
	ASSERT_NE(catalogues->addPC((testpaths::repoRoot() / "ProgramData" / "PC" / "S111_Portrayal" / "portrayal_catalogue.xml").wstring()), nullptr);
	const RECT view{ 0, 0, kWidth, kHeight };
	gis.SetScreen(view);
	ASSERT_TRUE(gis.AddLayer(s111.wstring()));
	auto* layer = gis.GetLayerManager()->GetLayer(gis.GetLayerManager()->LayerCount() - 1);
	auto* dataset = dynamic_cast<CoverageDataset*>(layer->m_spatialObject);
	ASSERT_NE(dataset, nullptr) << "S-111 opens as a coverage dataset";
	ASSERT_FALSE(dataset->GetInstructions().empty()) << "the XSLT rules give a coverage instruction";
	const auto& instruction = dataset->GetInstructions().front();
	EXPECT_EQ(instruction.attributeCode, "surfaceCurrentSpeed");
	ASSERT_EQ(instruction.lookup.size(), 9u) << "nine speed bands";
	ASSERT_TRUE(instruction.lookup.front().symbol);
	EXPECT_EQ(instruction.lookup.front().symbol->reference, "SCAROW01");
	EXPECT_EQ(instruction.lookup.front().symbol->rotationAttribute, "surfaceCurrentDirection");
	EXPECT_DOUBLE_EQ(instruction.lookup.front().symbol->defaultScaleFactor, 0.4);
	ASSERT_TRUE(instruction.lookup[4].symbol);
	EXPECT_EQ(instruction.lookup[4].symbol->scaleAttribute, "surfaceCurrentSpeed");
	EXPECT_DOUBLE_EQ(instruction.lookup[4].symbol->scaleFactor, 0.2);

	const auto span = CoverageDataset::DisplayTimeSpan();
	ASSERT_TRUE(span) << "the S-111 records are known";

	struct Frame
	{
		CoverageDataset::SymbolStatus status;
		std::size_t nonWhite = 0;
		std::size_t colours = 0;
	};
	const auto render = [&](const char* name) {
		MemoryCanvas canvas(kWidth, kHeight);
		gis.Draw(canvas.dc());
		::GdiFlush();
		Frame frame{ .status = dataset->LastSymbolStatus() };
		canvas.Measure(frame.nonWhite, frame.colours);
		const auto png = OutputDirectory() / (std::string("s111-") + name + ".png");
		EXPECT_TRUE(canvas.SavePng(png));
		std::cout << "[ s111 ] " << name << ": 1:" << gis.GetScaler()->GetCurrentScale() << ", every " << frame.status.thinningFactor
			<< " node(s), " << frame.status.drawn << " arrows, non-white " << frame.nonWhite << ", colours " << frame.colours
			<< ", " << png.string() << std::endl;
		return frame;
	};

	gis.SetMap(layer->GetMBR());

	// No time set and no record at the system clock: the first record is drawn, and reported.
	CoverageDataset::SetDisplaySettings({});
	EXPECT_EQ(CoverageDataset::CurrentDisplayTimeStatus().atFirstRecord, 1u);
	EXPECT_EQ(CoverageDataset::CurrentDisplayTimeStatus().firstRecordShown, span->first);
	EXPECT_EQ(CoverageDataset::CurrentDisplayTimeStatus().withoutRecord, 0u);
	EXPECT_GT(render("system-clock").status.drawn, 0u);

	// Clause 9.4: nothing before the first record at a set time.
	CoverageDataset::SetDisplaySettings({ .time = span->first - std::chrono::hours(1) });
	EXPECT_EQ(CoverageDataset::CurrentDisplayTimeStatus().datasets, 1u);
	EXPECT_EQ(CoverageDataset::CurrentDisplayTimeStatus().withoutRecord, 1u) << "the viewer says why nothing is drawn";
	const Frame before = render("before");
	EXPECT_EQ(before.status.drawn, 0u);

	// At the first record, fitted to the screen. The Korean grid is coarse (about 2 km): its
	// arrows have room at this scale, and every grid point is drawn.
	CoverageDataset::SetDisplaySettings({ .time = span->first });
	EXPECT_EQ(CoverageDataset::CurrentDisplayTimeStatus().withoutRecord, 0u);
	const Frame fitted = render("fitted");
	EXPECT_GT(fitted.status.drawn, 0u);
	EXPECT_EQ(fitted.status.thinningFactor, 1);
	EXPECT_GT(fitted.nonWhite, before.nonWhite);
	EXPECT_GE(fitted.colours, 2u) << "white and at least one speed band";

	// Zooming out brings the grid points together: fewer and fewer rows and columns are drawn.
	int factor = 1;
	Frame thinned = fitted;
	for (int step = 0; step < 4; ++step)
	{
		gis.GetScaler()->ZoomOut(100);
		thinned = render(("out-" + std::to_string(step + 1)).c_str());
		EXPECT_GE(thinned.status.thinningFactor, factor) << "coarser as the chart is zoomed out";
		factor = thinned.status.thinningFactor;
	}
	EXPECT_GT(thinned.status.thinningFactor, 2) << "at a sixteenth of the size the arrows would run over each other";
	EXPECT_GT(thinned.status.drawn, 0u);

	// The same view without thinning: every grid point, which is far more arrows.
	CoverageDataset::SetDisplaySettings({ .time = span->first, .thinSymbols = false });
	const Frame dense = render("out-unthinned");
	EXPECT_EQ(dense.status.thinningFactor, 1);
	EXPECT_GT(dense.status.drawn, thinned.status.drawn * static_cast<std::size_t>(thinned.status.thinningFactor));
	EXPECT_GT(dense.nonWhite, thinned.nonWhite);

	// Zooming back in thins less, down to every grid point again.
	CoverageDataset::SetDisplaySettings({ .time = span->first });
	Frame zoomed = thinned;
	for (int step = 0; step < 12 && zoomed.status.thinningFactor > 1; ++step)
	{
		gis.GetScaler()->ZoomIn(50);
		zoomed = render(("in-" + std::to_string(step + 1)).c_str());
		EXPECT_LE(zoomed.status.thinningFactor, factor);
		factor = zoomed.status.thinningFactor;
	}
	EXPECT_EQ(zoomed.status.thinningFactor, 1) << "zoomed in far enough, nothing is left out";

	// The pick report gives the speed and direction under a point with data.
	const auto& feature = dataset->GetCoverage()->dataset().features.front();
	const auto speedIndex = feature.componentIndex("surfaceCurrentSpeed");
	ASSERT_TRUE(speedIndex);
	const auto speeds = dataset->GetCoverage()->readValues(feature.id, { .timeIndex = 0, .component = *speedIndex });
	ASSERT_TRUE(speeds);
	const double fill = feature.components[*speedIndex].fillValue;
	const auto valid = std::ranges::find_if(*speeds, [&](float value) { return !s100::coverage::isNoData(value, fill) && value > 0; });
	ASSERT_NE(valid, speeds->end());
	const auto index = static_cast<int>(valid - speeds->begin());
	const auto section = dataset->PickAt(s100::coverage::gridPosition(feature.grid, index % feature.grid.cols, index / feature.grid.cols));
	CoverageDataset::SetDisplaySettings(original);
	ASSERT_TRUE(section);
	EXPECT_EQ(section->product, "S-111");
	for (const auto& [name, value] : section->rows)
	{
		std::cout << "[ s111 pick ] " << name << ": " << value << std::endl;
	}
	EXPECT_GE(section->rows.size(), 3u) << "time, speed and direction";
}

// S-111 through the System Database, as the viewer shows it: installed with its catalogues, picked
// by the chart for the view and drawn as arrows, alone and over an S-101 cell with S-102 and S-104.
TEST(RenderSmoke, SystemDatabaseChartDrawsS111)
{
	const auto tds = testpaths::tdsRoot() / L"KOR_TDS";
	const auto s111 = tds / L"KOR_S-111" / L"S-111" / L"111KR00KR4_MOHID2K_G3B30_20260330_F36.h5";
	const auto s104 = tds / L"KOR_S-104" / L"S-104" / L"104KR00KR4_MOHID2K_G3B30_20260330_F36.h5";
	const auto s102 = tds / L"KOR_S-102" / L"S-102" / L"102KR00T4G3B3000026.h5";
	const auto cell = tds / L"S100_ROOT" / L"S-101" / L"DATASET_FILES" / L"101KR005G3B33" / L"101KR005G3B33.000";
	const auto s102pc = testpaths::webDataRoot().parent_path() / L"pc" / L"S102_Portrayal";
	for (const auto& path : { s111, s104, s102, cell, s102pc })
	{
		if (!std::filesystem::exists(path))
		{
			GTEST_SKIP() << "test data not available: " << path.string();
		}
	}
	using GISLibrary::CoverageDataset;
	const CoverageDataset::DisplaySettings original = CoverageDataset::GetDisplaySettings();
	CoverageDataset::SetDisplaySettings({});

	const auto install = [&](const std::filesystem::path& root, bool withCharts) {
		std::filesystem::remove_all(root);
		GISLibrary::SystemDatabaseSession session(root);
		auto& importer = session.importer();
		const auto fc = testpaths::repoRoot() / "ProgramData" / "FC";
		const auto pc = testpaths::repoRoot() / "ProgramData" / "PC";
		ASSERT_EQ(importer.importFeatureCatalogue(fc / "S-111_FC.xml").outcome, s100::sdb::ImportOutcome::Added);
		ASSERT_EQ(importer.importPortrayalCatalogue(pc / "S111_Portrayal").outcome, s100::sdb::ImportOutcome::Added);
		ASSERT_EQ(importer.importDataset(s111).outcome, s100::sdb::ImportOutcome::Added);
		if (withCharts)
		{
			ASSERT_EQ(importer.importFeatureCatalogue(fc / "S-101_FC.xml").outcome, s100::sdb::ImportOutcome::Added);
			ASSERT_EQ(importer.importPortrayalCatalogue(pc / "S101_Portrayal").outcome, s100::sdb::ImportOutcome::Added);
			ASSERT_EQ(importer.importFeatureCatalogue(fc / "S-102_FC.xml").outcome, s100::sdb::ImportOutcome::Added);
			ASSERT_EQ(importer.importPortrayalCatalogue(s102pc).outcome, s100::sdb::ImportOutcome::Added);
			ASSERT_EQ(importer.importFeatureCatalogue(fc / "S-104_FC.xml").outcome, s100::sdb::ImportOutcome::Added);
			ASSERT_EQ(importer.importDataset(cell).outcome, s100::sdb::ImportOutcome::Added);
			ASSERT_EQ(importer.importDataset(s102).outcome, s100::sdb::ImportOutcome::Added);
			ASSERT_EQ(importer.importDataset(s104).outcome, s100::sdb::ImportOutcome::Added);
		}
	};

	GdiplusSession gdiplus;
	const auto draw = [&](const std::filesystem::path& root, const char* name) {
		CGISLibraryApp gis;
		auto* catalogues = gis.GetCatalogManager();
		catalogues->onPortrayalLoaded = [&gis](PortrayalCatalogue* pc) { GISLibrary::PortrayalRender::Prepare(pc, gis.GetD2D1Resources()); };
		const RECT view{ 0, 0, kWidth, kHeight };
		gis.SetScreen(view);
		GISLibrary::SystemDatabaseSession session(root);
		session.attach(*gis.GetLayerManager(), *catalogues);
		// Off Busan, a few grid nodes across: the view of the report that S-111 was not drawn.
		MBR area(129.12, 35.11, 129.21, 35.17);
		projection(area);
		gis.SetMap(area);
		MemoryCanvas canvas(kWidth, kHeight);
		gis.Draw(canvas.dc());
		::GdiFlush();
		EXPECT_TRUE(canvas.SavePng(OutputDirectory() / (std::string("sdb-chart-s111-") + name + ".png")));
		std::size_t arrows = 0;
		std::size_t datasets = 0;
		if (auto* inner = session.chart()->GetInnerLayers())
		{
			for (int i = 0; i < inner->LayerCount(); ++i)
			{
				auto* coverage = dynamic_cast<CoverageDataset*>(inner->GetLayer(i)->GetSpatialObject());
				if (coverage && coverage->GetCoverage() && coverage->GetCoverage()->dataset().productId == "S-111")
				{
					++datasets;
					arrows += coverage->LastSymbolStatus().drawn;
					std::cout << "[ sdb chart ] S-111 " << name << ": instructions " << coverage->GetInstructions().size() << ", arrows "
						<< coverage->LastSymbolStatus().drawn << ", every " << coverage->LastSymbolStatus().thinningFactor << std::endl;
				}
			}
		}
		std::cout << "[ sdb chart ] S-111 " << name << ": 1:" << gis.GetScaler()->GetCurrentScale() << ", shown "
			<< session.chart()->GetLayerSet().shownCount() << ", S-111 layers " << datasets << ", arrows " << arrows << std::endl;
		EXPECT_EQ(datasets, 1u) << "the chart opens the S-111 in view";
		EXPECT_GT(arrows, 0u) << "its arrows are drawn";
		// What the arrows change on screen: the same view drawn again with the S-111 hidden by a
		// time before its records.
		CoverageDataset::SetDisplaySettings({ .time = s100::coverage::TimePoint{} });
		MemoryCanvas without(kWidth, kHeight);
		gis.Draw(without.dc());
		::GdiFlush();
		CoverageDataset::SetDisplaySettings({});
		const std::size_t changed = without.DifferingPixels(canvas);
		std::cout << "[ sdb chart ] S-111 " << name << ": pixels the arrows change " << changed << std::endl;
		EXPECT_GT(changed, 200u) << "the arrows show, over whatever else the chart draws";
	};
	const auto alone = testpaths::tempDir() / L"SystemDatabaseChartS111";
	const auto withCharts = testpaths::tempDir() / L"SystemDatabaseChartS111Charts";
	install(alone, false);
	draw(alone, "alone");
	install(withCharts, true);
	draw(withCharts, "charts");
	CoverageDataset::SetDisplaySettings(original);
}

// Coverage backport CV-6: water level adjustment in the viewer. KOR S-102 and the S-104 over it,
// opened as layers; with WLA at the first S-104 record (their datums differ, so only with the
// datum check overridden, as opens100-web's viewer offers for test data) the depths change.
TEST(RenderSmoke, AdjustsS102ByTheS104OnScreen)
{
	const auto s102 = testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-102" / L"S-102" / L"102KR00T4G3B3000026.h5";
	const auto s104 = testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-104" / L"S-104" / L"104KR00KR4_MOHID2K_G3B30_20260330_F36.h5";
	const auto pc = testpaths::webDataRoot().parent_path() / L"pc" / L"S102_Portrayal" / L"portrayal_catalogue.xml";
	if (!std::filesystem::exists(s102) || !std::filesystem::exists(s104) || !std::filesystem::exists(pc))
	{
		GTEST_SKIP() << "KOR S-102/S-104 or the S-102 PC not available";
	}
	using GISLibrary::CoverageDataset;
	const CoverageDataset::DepthSettings original = CoverageDataset::GetDepthSettings();

	GdiplusSession gdiplus;
	CGISLibraryApp gis;
	auto* catalogues = gis.GetCatalogManager();
	ASSERT_NE(catalogues->addFC((testpaths::repoRoot() / "ProgramData" / "FC" / "S-102_FC.xml").string()), nullptr);
	ASSERT_NE(catalogues->addFC((testpaths::repoRoot() / "ProgramData" / "FC" / "S-104_FC.xml").string()), nullptr);
	ASSERT_NE(catalogues->addPC(pc.wstring()), nullptr);
	const RECT view{ 0, 0, kWidth, kHeight };
	gis.SetScreen(view);
	ASSERT_TRUE(gis.AddLayer(s104.wstring()));
	ASSERT_TRUE(gis.AddLayer(s102.wstring()));
	ASSERT_TRUE(CoverageDataset::WaterLevelTimeSpan()) << "the S-104 records are known";

	const auto render = [&](const char* name, std::vector<std::uint32_t>& pixels) {
		// Fit the S-102 layer: the last one added.
		gis.SetMap(gis.GetLayerManager()->GetLayer(gis.GetLayerManager()->LayerCount() - 1)->GetMBR());
		MemoryCanvas canvas(kWidth, kHeight);
		gis.Draw(canvas.dc());
		::GdiFlush();
		const auto png = OutputDirectory() / (std::string("s102-wla-") + name + ".png");
		EXPECT_TRUE(canvas.SavePng(png));
		std::size_t nonWhite = 0, colours = 0;
		canvas.Measure(nonWhite, colours);
		std::cout << "[ wla ] " << name << ": non-white " << nonWhite << ", colours " << colours << ", " << png.string() << std::endl;
		pixels.assign(static_cast<std::size_t>(kWidth) * kHeight, 0);
		BITMAPINFO info{};
		info.bmiHeader = { sizeof(BITMAPINFOHEADER), kWidth, -kHeight, 1, 32, BI_RGB };
		HBITMAP bitmap = static_cast<HBITMAP>(::GetCurrentObject(canvas.dc(), OBJ_BITMAP));
		::GetDIBits(canvas.dc(), bitmap, 0, kHeight, pixels.data(), &info, DIB_RGB_COLORS);
	};

	std::vector<std::uint32_t> plain;
	render("off", plain);

	CoverageDataset::DepthSettings wla = original;
	wla.adjustment.wla = s100::coverage::WlaMethod::instant(CoverageDataset::WaterLevelTimeSpan()->first);
	wla.adjustment.ignoreIncompatibleDatums = true;
	CoverageDataset::SetDepthSettings(wla);
	std::vector<std::uint32_t> adjusted;
	render("instant", adjusted);
	const auto status = CoverageDataset::CurrentWlaStatus();

	// The pick report at a cell WLA adjusted: the file's depth, the water level and the adjusted
	// depth (S-98 D-2 item 3b), and the portrayal entry.
	auto* dataset = dynamic_cast<CoverageDataset*>(gis.GetLayerManager()->GetLayer(gis.GetLayerManager()->LayerCount() - 1)->m_spatialObject);
	ASSERT_NE(dataset, nullptr);
	const auto* source = dynamic_cast<const s100::coverage::WaterLevelAdjustedCoverage*>(dataset->GetCoverage());
	ASSERT_NE(source, nullptr);
	const auto& feature = source->dataset().features.front();
	const auto* adjustment = source->adjustment(feature.id);
	ASSERT_NE(adjustment, nullptr);
	const auto applied = std::ranges::find(adjustment->applied, std::uint8_t{ 1 });
	ASSERT_NE(applied, adjustment->applied.end());
	const auto index = static_cast<int>(applied - adjustment->applied.begin());
	const auto position = s100::coverage::gridPosition(feature.grid, index % feature.grid.cols, index / feature.grid.cols);
	const auto sections = CoverageDataset::PickAllAt(position);
	CoverageDataset::SetDepthSettings(original);
	ASSERT_EQ(sections.size(), 1u);
	const auto& section = sections.front();
	EXPECT_EQ(section.product, "S-102");
	std::map<std::string, std::string> rows(section.rows.begin(), section.rows.end());
	for (const auto& [name, value] : section.rows)
	{
		std::cout << "[ wla pick ] " << name << ": " << value << std::endl;
	}
	EXPECT_TRUE(rows.contains("Water level"));
	EXPECT_TRUE(rows.contains("Portrayal"));
	ASSERT_TRUE(rows.contains("Depth (adjusted)"));
	EXPECT_NE(rows["Depth (adjusted)"].find("[WLA "), std::string::npos);
	const double raw = (*source->base().readValues(feature.id, {}))[index];
	const double level = adjustment->waterLevel[index];
	EXPECT_EQ(rows["Depth (adjusted)"].substr(0, rows["Depth (adjusted)"].find(' ')), std::format("{:.2f}", (*adjustment->depth)[index]));
	std::cout << "[ wla pick ] raw " << raw << ", water level " << level << std::endl;
	EXPECT_TRUE(CoverageDataset::PickAllAt({ .lon = 0, .lat = 0 }).empty()) << "nothing outside the data";

	EXPECT_GT(status.applied, 0u);
	EXPECT_TRUE(status.issues.contains(s100::coverage::WlaIssue::IncompatibleDatums));
	const auto changed = std::inner_product(plain.begin(), plain.end(), adjusted.begin(), std::size_t{ 0 }, std::plus<>(),
		[](std::uint32_t a, std::uint32_t b) { return a != b ? std::size_t{ 1 } : std::size_t{ 0 }; });
	std::cout << "[ wla ] pixels changed by WLA: " << changed << std::endl;
	EXPECT_GT(changed, 0u) << "adjusted depths draw differently (shading, contour, WLA boundary)";
}

// Coverage backport CV-4: with the enhanced safety contour on, the S-101 depth features
// (DepthArea, DredgedArea, DepthContour) are not drawn where S-102 data covers the water (S-98 D-1).
TEST(RenderSmoke, SuppressesS101DepthFeaturesUnderS102)
{
	const auto cell = testpaths::tdsRoot() / L"KOR_TDS" / L"S100_ROOT" / L"S-101" / L"DATASET_FILES" / L"101KR004G3B30" / L"101KR004G3B30.000";
	const auto s102 = testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-102" / L"S-102" / L"102KR00T4G3B3000026.h5";
	if (!std::filesystem::exists(cell) || !std::filesystem::exists(s102))
	{
		GTEST_SKIP() << "KOR S-101/S-102 not available";
	}
	using GISLibrary::CoverageDataset;
	const CoverageDataset::DepthSettings original = CoverageDataset::GetDepthSettings();

	GdiplusSession gdiplus;
	CGISLibraryApp gis;
	auto* catalogues = gis.GetCatalogManager();
	ASSERT_NE(catalogues->addFC((testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml").string()), nullptr);
	auto* s101pc = catalogues->addPC((testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml").wstring());
	ASSERT_NE(s101pc, nullptr);
	GISLibrary::PortrayalRender::Prepare(s101pc, gis.GetD2D1Resources());
	// The S-102 without its portrayal catalogue: open, with its data outline, but not drawn, so what
	// changes with ESC on is the S-101 depth portrayal alone.
	ASSERT_NE(catalogues->addFC((testpaths::repoRoot() / "ProgramData" / "FC" / "S-102_FC.xml").string()), nullptr);
	const RECT view{ 0, 0, kWidth, kHeight };
	gis.SetScreen(view);
	ASSERT_TRUE(gis.AddLayer(cell.wstring()));
	ASSERT_TRUE(gis.AddLayer(s102.wstring()));
	gis.SetMap(gis.GetLayerManager()->GetLayer(0)->GetMBR());

	const auto render = [&](bool esc, const char* name) {
		CoverageDataset::DepthSettings settings = original;
		settings.enhancedSafetyContour = esc;
		CoverageDataset::SetDepthSettings(settings);
		MemoryCanvas canvas(kWidth, kHeight);
		gis.Draw(canvas.dc());
		::GdiFlush();
		const auto png = OutputDirectory() / (std::string("s101-under-s102-") + name + ".png");
		EXPECT_TRUE(canvas.SavePng(png));
		std::cout << "[ suppression ] " << name << ": " << png.string() << std::endl;
		std::vector<std::uint32_t> pixels(static_cast<std::size_t>(kWidth) * kHeight, 0);
		BITMAPINFO info{};
		info.bmiHeader = { sizeof(BITMAPINFOHEADER), kWidth, -kHeight, 1, 32, BI_RGB };
		::GetDIBits(canvas.dc(), static_cast<HBITMAP>(::GetCurrentObject(canvas.dc(), OBJ_BITMAP)), 0, kHeight, pixels.data(), &info, DIB_RGB_COLORS);
		return pixels;
	};
	const auto off = render(false, "esc-off");
	const auto on = render(true, "esc-on");
	// The S-102 data outlines on the screen, as a bounding box: nothing outside it may change.
	const auto rings = CoverageDataset::SuppressionRings();
	long left = kWidth, top = kHeight, right = 0, bottom = 0;
	for (const auto& ring : rings)
	{
		for (const auto& position : ring)
		{
			double x = position.lon;
			double y = position.lat;
			projection(x, y);
			long sx = 0, sy = 0;
			gis.WorldToDevice(x, y, &sx, &sy);
			left = (std::min)(left, sx);
			right = (std::max)(right, sx);
			top = (std::min)(top, sy);
			bottom = (std::max)(bottom, sy);
		}
	}
	CoverageDataset::SetDepthSettings(original);

	EXPECT_FALSE(CoverageDataset::SuppressionVersion() == 0);
	// A clear difference, not the rounding of drawing through a layer.
	const auto differs = [](std::uint32_t a, std::uint32_t b) {
		for (int shift : { 0, 8, 16 })
		{
			if (std::abs(static_cast<int>((a >> shift) & 0xFF) - static_cast<int>((b >> shift) & 0xFF)) > 8) return true;
		}
		return false;
	};
	const auto changed = std::inner_product(off.begin(), off.end(), on.begin(), std::size_t{ 0 }, std::plus<>(),
		[&](std::uint32_t a, std::uint32_t b) { return differs(a, b) ? std::size_t{ 1 } : std::size_t{ 0 }; });
	std::cout << "[ suppression ] pixels that differ clearly with ESC on: " << changed << std::endl;
	EXPECT_GT(changed, 100000u) << "the S-101 depth areas under the S-102 data are not drawn";
	EXPECT_FALSE(rings.empty());
	// Outside the data (a few pixels of margin for antialiasing) the S-101 depth features stay.
	std::size_t outside = 0;
	for (int y = 0; y < kHeight; ++y)
	{
		for (int x = 0; x < kWidth; ++x)
		{
			const bool beyond = x < left - 3 || x > right + 3 || y < top - 3 || y > bottom + 3;
			const auto i = static_cast<std::size_t>(y) * kWidth + x;
			if (beyond && differs(off[i], on[i])) ++outside;
		}
	}
	std::cout << "[ suppression ] S-102 data on screen: " << left << "," << top << " - " << right << "," << bottom
		<< "; differing pixels outside it: " << outside << std::endl;
	EXPECT_EQ(outside, 0u) << "S-101 depth features outside the S-102 data are suppressed too";

	// Where they differ, in red over the ESC-on picture, for a person to look at.
	std::vector<std::uint32_t> diff(on.size());
	for (std::size_t i = 0; i < on.size(); ++i)
	{
		diff[i] = differs(on[i], off[i]) ? 0x00FF0000u : ((on[i] >> 1) & 0x007F7F7Fu) | 0x00808080u;
	}
	MemoryCanvas canvas(kWidth, kHeight);
	BITMAPINFO info{};
	info.bmiHeader = { sizeof(BITMAPINFOHEADER), kWidth, -kHeight, 1, 32, BI_RGB };
	::SetDIBitsToDevice(canvas.dc(), 0, 0, kWidth, kHeight, 0, 0, 0, kHeight, diff.data(), &info, DIB_RGB_COLORS);
	const auto png = OutputDirectory() / "s101-under-s102-diff.png";
	EXPECT_TRUE(canvas.SavePng(png));
	std::cout << "[ suppression ] differences: " << png.string() << std::endl;
}


// Coverage backport CV-7: KOR S-102 and S-104 with the Busan S-101 cells through the System
// Database chart, drawn at several scales with the enhanced safety contour and with WLA, each view
// timed on its first frame (datasets opened, portrayed, adjusted) and on the next one. The System
// Database is left in place for a run of the viewer (OPENS100_SDB_ROOT=<temp>\OpenS100Tests\CoverageChart).
TEST(RenderSmoke, CoverageChartAtScales)
{
	const auto cells = testpaths::tdsRoot() / L"KOR_TDS" / L"S100_ROOT" / L"S-101" / L"DATASET_FILES";
	const auto s102 = testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-102" / L"S-102" / L"102KR00T4G3B3000026.h5";
	const auto s104 = testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-104" / L"S-104" / L"104KR00KR4_MOHID2K_G3B30_20260330_F36.h5";
	const auto pcZip = testpaths::tdsRoot() / L"FC_PC" / L"S-102" / L"unzipped" / L"S-102" / L"CATALOGUES" / L"102_Portrayal_Catalogue_3.0.0.zip";
	for (const auto& path : { cells, s102, s104, pcZip })
	{
		if (!std::filesystem::exists(path))
		{
			GTEST_SKIP() << "not available: " << path.string();
		}
	}
	using GISLibrary::CoverageDataset;
	using Clock = std::chrono::steady_clock;
	const auto root = testpaths::tempDir() / L"CoverageChart";
	std::filesystem::remove_all(root);
	const char* busan[] = { "101KR00659C81", "101KR00659C82", "101KR005G3B33", "101KR004G3B30", "101KR003G3B00", "101KR002G3000", "101KR001G0000" };
	{
		GISLibrary::SystemDatabaseSession session(root);
		auto& importer = session.importer();
		const auto fc = testpaths::repoRoot() / "ProgramData" / "FC";
		ASSERT_EQ(importer.importFeatureCatalogue(fc / "S-101_FC.xml").outcome, s100::sdb::ImportOutcome::Added);
		ASSERT_EQ(importer.importPortrayalCatalogue(testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal").outcome, s100::sdb::ImportOutcome::Added);
		ASSERT_EQ(importer.importFeatureCatalogue(fc / "S-102_FC.xml").outcome, s100::sdb::ImportOutcome::Added);
		ASSERT_EQ(importer.importPortrayalCatalogue(pcZip).outcome, s100::sdb::ImportOutcome::Added);
		ASSERT_EQ(importer.importFeatureCatalogue(fc / "S-104_FC.xml").outcome, s100::sdb::ImportOutcome::Added);
		for (const char* name : busan)
		{
			ASSERT_EQ(importer.importDataset(cells / name / (std::string(name) + ".000")).outcome, s100::sdb::ImportOutcome::Added) << name;
		}
		ASSERT_EQ(importer.importDataset(s102).outcome, s100::sdb::ImportOutcome::Added);
		ASSERT_EQ(importer.importDataset(s104).outcome, s100::sdb::ImportOutcome::Added);
	}

	GdiplusSession gdiplus;
	CGISLibraryApp gis;
	auto* catalogues = gis.GetCatalogManager();
	catalogues->onPortrayalLoaded = [&gis](PortrayalCatalogue* pc) { GISLibrary::PortrayalRender::Prepare(pc, gis.GetD2D1Resources()); };
	const RECT view{ 0, 0, kWidth, kHeight };
	gis.SetScreen(view);
	GISLibrary::SystemDatabaseSession session(root);
	session.attach(*gis.GetLayerManager(), *catalogues);
	const auto record = session.database().datasets().find("S-102", "102KR00T4G3B3000026");
	ASSERT_TRUE(record && record->bounds);
	const double cx = (record->bounds->minX + record->bounds->maxX) / 2;
	const double cy = (record->bounds->minY + record->bounds->maxY) / 2;
	const double hw = (record->bounds->maxX - record->bounds->minX) / 2;
	const double hh = (record->bounds->maxY - record->bounds->minY) / 2;

	const CoverageDataset::DepthSettings original = CoverageDataset::GetDepthSettings();
	const auto ms = [](Clock::time_point a, Clock::time_point b) { return std::chrono::duration<double, std::milli>(b - a).count(); };
	// The S-102 extent zoomed by `zoom` (1 fits it), drawn twice.
	const auto frame = [&](double zoom, const CoverageDataset::DepthSettings& settings, const std::string& name) {
		CoverageDataset::SetDepthSettings(settings);
		MBR area(cx - hw / zoom, cy - hh / zoom, cx + hw / zoom, cy + hh / zoom);
		projection(area);
		gis.SetMap(area);
		double times[2] = {};
		std::size_t nonWhite = 0;
		std::size_t colours = 0;
		for (double& time : times)
		{
			MemoryCanvas canvas(kWidth, kHeight);
			const auto start = Clock::now();
			gis.Draw(canvas.dc());
			::GdiFlush();
			time = ms(start, Clock::now());
			if (&time == &times[1])
			{
				canvas.Measure(nonWhite, colours);
				EXPECT_TRUE(canvas.SavePng(OutputDirectory() / ("cv7-" + name + ".png")));
			}
		}
		std::cout << "[ cv-7 ] " << name << " 1:" << static_cast<long long>(gis.GetScaler()->GetCurrentScale()) << "  first "
			<< std::format("{:.0f}", times[0]) << " ms, next " << std::format("{:.0f}", times[1]) << " ms, colours " << colours << std::endl;
		EXPECT_GT(nonWhite, static_cast<std::size_t>(kWidth) * kHeight / 50) << name;
	};

	CoverageDataset::DepthSettings esc = original;
	esc.enhancedSafetyContour = true;
	esc.adjustment = {};
	CoverageDataset::DepthSettings plain = esc;
	plain.enhancedSafetyContour = false;
	frame(1, plain, "fit-esc-off");
	frame(1, esc, "fit-esc-on");
	frame(0.25, esc, "out4-esc-on");
	frame(4, esc, "in4-esc-on");
	frame(16, esc, "in16-esc-on");
	const auto span = CoverageDataset::WaterLevelTimeSpan();
	ASSERT_TRUE(span) << "the S-104 is opened with the S-102";
	CoverageDataset::DepthSettings wla = esc;
	wla.adjustment.wla = s100::coverage::WlaMethod::instant(span->first);
	wla.adjustment.ignoreIncompatibleDatums = true;
	frame(1, wla, "fit-wla-instant");
	frame(4, wla, "in4-wla-instant");
	wla.adjustment.wla = s100::coverage::WlaMethod::period(span->first, span->first + std::chrono::hours(6));
	wla.adjustment.uncertainty = true;
	frame(1, wla, "fit-wla-period-uncertainty");
	EXPECT_GT(CoverageDataset::CurrentWlaStatus().applied, 0u);
	CoverageDataset::SetDepthSettings(original);
	session.detach();
}

// S-124 and S-128 GML datasets installed in the System Database are drawn by the chart where they
// lie: the KOR S-124 warning area off Busan North Port, the KOR S-128 product outlines.
namespace
{
	void DrawGmlFromSystemDatabase(const char* product, const wchar_t* pcFolder, const std::filesystem::path& file)
	{
		if (!std::filesystem::exists(file))
		{
			GTEST_SKIP() << product << " test data not available: " << file.string();
		}
		const auto root = testpaths::tempDir() / (L"SystemDatabaseChartGml-" + file.stem().wstring());
		std::filesystem::remove_all(root);
		{
			GISLibrary::SystemDatabaseSession session(root);
			const auto fc = std::string(product) + "_FC.xml";
			ASSERT_EQ(session.importer().importFeatureCatalogue(testpaths::repoRoot() / "ProgramData" / "FC" / fc).outcome,
				s100::sdb::ImportOutcome::Added);
			ASSERT_EQ(session.importer().importPortrayalCatalogue(testpaths::repoRoot() / "ProgramData" / "PC" / pcFolder).outcome,
				s100::sdb::ImportOutcome::Added);
			const auto imported = session.importer().importDataset(file);
			ASSERT_EQ(imported.outcome, s100::sdb::ImportOutcome::Added) << imported.message;
		}

		GdiplusSession gdiplus;
		CGISLibraryApp gis;
		auto* catalogues = gis.GetCatalogManager();
		catalogues->onPortrayalLoaded = [&gis](PortrayalCatalogue* pc) { GISLibrary::PortrayalRender::Prepare(pc, gis.GetD2D1Resources()); };
		const RECT view{ 0, 0, kWidth, kHeight };
		gis.SetScreen(view);

		GISLibrary::SystemDatabaseSession session(root);
		session.attach(*gis.GetLayerManager(), *catalogues);
		auto* chart = session.chart();
		ASSERT_NE(chart, nullptr);
		const auto record = session.database().datasets().find(product, file.stem().string());
		ASSERT_TRUE(record && record->bounds);

		// The dataset with a margin of its own size each side.
		const double width = record->bounds->maxX - record->bounds->minX;
		const double height = record->bounds->maxY - record->bounds->minY;
		MBR area(record->bounds->minX - width, record->bounds->minY - height, record->bounds->maxX + width, record->bounds->maxY + height);
		projection(area);
		gis.SetMap(area);
		MemoryCanvas canvas(kWidth, kHeight);
		gis.Draw(canvas.dc());
		std::size_t nonWhite = 0;
		std::size_t colours = 0;
		canvas.Measure(nonWhite, colours);
		const auto png = OutputDirectory() / ("sdb-chart-" + file.stem().string() + ".png");
		EXPECT_TRUE(canvas.SavePng(png));
		std::cout << "[ sdb chart ] " << product << " 1:" << gis.GetScaler()->GetCurrentScale() << " opened "
			<< chart->GetLayerSet().openCount() << ", non-white " << nonWhite << ", colours " << colours << ", " << png.string() << std::endl;
		// The viewer reads the geometry where the importer found it: the layer lies within the
		// dataset's bounds (in the view's middle third).
		auto* inner = chart->GetInnerLayers();
		ASSERT_TRUE(inner && inner->LayerCount() == 1);
		auto* gml = dynamic_cast<S10XGML*>(inner->GetLayer(0)->GetSpatialObject());
		ASSERT_NE(gml, nullptr);
		EXPECT_GT(gml->geometries.size(), 0u);
		MBR bounds(record->bounds->minX, record->bounds->minY, record->bounds->maxX, record->bounds->maxY);
		projection(bounds);
		const MBR drawn = inner->GetLayer(0)->GetMBR();
		const double tolerance = 1e-6 + (bounds.xmax - bounds.xmin) * 0.01;
		EXPECT_GE(drawn.xmin, bounds.xmin - tolerance);
		EXPECT_LE(drawn.xmax, bounds.xmax + tolerance);
		EXPECT_GE(drawn.ymin, bounds.ymin - tolerance);
		EXPECT_LE(drawn.ymax, bounds.ymax + tolerance);
		EXPECT_TRUE(chart->GetLayerSet().isShown(record->id)) << "the dataset is in view";
		EXPECT_EQ(chart->GetLayerSet().openCount(), 1u) << "opened with its catalogues";
		EXPECT_GT(nonWhite, 500u) << "something is drawn";
	}
}

TEST(RenderSmoke, SystemDatabaseChartDrawsS124Gml)
{
	DrawGmlFromSystemDatabase("S-124", L"S124_Portrayal", testpaths::korS124Gml());
}

// TEST2 is a circle of 185.2 m (gml:CircleByCenterPoint) off Busan.
TEST(RenderSmoke, SystemDatabaseChartDrawsS124Circle)
{
	DrawGmlFromSystemDatabase("S-124", L"S124_Portrayal",
		testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-124" / L"S-124" / L"124KR.KCG.XI.S100.TEST2.gml");
}

TEST(RenderSmoke, SystemDatabaseChartDrawsS128Gml)
{
	DrawGmlFromSystemDatabase("S-128", L"S128_Portrayal",
		testpaths::tdsRoot() / L"KOR_TDS" / L"KOR_S-128" / L"S-128" / L"128AA0020260522150149.gml");
}

// Two adjacent cells of one scale (101KR005G3A44 and 101KR005G3B33 meet at 129 E): drawn one after
// the other, the second's area fills cover the symbols and text the first draws across the common
// edge, so the picture depends on which comes first. Drawn as one group, priority by priority
// across both, it does not (but for objects of one priority overlapping at the edge).
TEST(RenderSmoke, AdjacentCellsOfOneGroupDoNotCoverEachOther)
{
	const auto cells = testpaths::tdsRoot() / L"KOR_TDS" / L"S100_ROOT" / L"S-101" / L"DATASET_FILES";
	const auto west = cells / L"101KR005G3A44" / L"101KR005G3A44.000";
	const auto east = cells / L"101KR005G3B33" / L"101KR005G3B33.000";
	if (!std::filesystem::exists(west) || !std::filesystem::exists(east))
	{
		GTEST_SKIP() << "reference cells not available under " << cells.string();
	}

	GdiplusSession gdiplus;
	CGISLibraryApp gis;
	auto* catalogues = gis.GetCatalogManager();
	ASSERT_NE(catalogues->addFC((testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml").string()), nullptr);
	auto* pc = catalogues->addPC((testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml").wstring());
	ASSERT_NE(pc, nullptr);
	GISLibrary::PortrayalRender::Prepare(pc, gis.GetD2D1Resources());
	const RECT view{ 0, 0, kWidth, kHeight };
	gis.SetScreen(view);
	ASSERT_TRUE(gis.AddLayer(west.wstring()));
	ASSERT_TRUE(gis.AddLayer(east.wstring()));

	auto* layers = gis.GetLayerManager();
	layers->onIC = false;   // as the System Database chart draws
	layers->m_isScreenFitEnabled = false;
	ASSERT_EQ(layers->layers.size(), 2u);
	MBR area(128.96, 35.06, 129.04, 35.11);
	projection(area);
	gis.SetMap(area);

	const auto draw = [&](MemoryCanvas& canvas, bool reversed, bool grouped) {
		layers->layers.reverse();
		if (!reversed) layers->layers.reverse();
		layers->drawGroups.clear();
		if (grouped)
		{
			for (const Layer* layer : layers->layers) layers->drawGroups[layer] = 1;
		}
		gis.Draw(canvas.dc());
		::GdiFlush();
	};

	MemoryCanvas first(kWidth, kHeight);
	MemoryCanvas second(kWidth, kHeight);
	draw(first, false, false);
	draw(second, true, false);
	const std::size_t apart = first.DifferingPixels(second);
	EXPECT_TRUE(first.SavePng(OutputDirectory() / "adjacent-apart.png"));
	EXPECT_TRUE(second.SavePng(OutputDirectory() / "adjacent-apart-reversed.png"));

	MemoryCanvas groupedFirst(kWidth, kHeight);
	MemoryCanvas groupedSecond(kWidth, kHeight);
	draw(groupedFirst, false, true);
	draw(groupedSecond, true, true);
	const std::size_t together = groupedFirst.DifferingPixels(groupedSecond);
	EXPECT_TRUE(groupedFirst.SavePng(OutputDirectory() / "adjacent-grouped.png"));
	EXPECT_TRUE(groupedSecond.SavePng(OutputDirectory() / "adjacent-grouped-reversed.png"));

	std::cout << "[ adjacent ] 1:" << gis.GetScaler()->GetCurrentScale() << " pixels that depend on the order: one at a time "
		<< apart << ", as one group " << together << std::endl;
	EXPECT_GT(apart, 200u) << "drawn one at a time, the second cell covers the first's symbols at the edge";
	// What is left are features both cells hold at the edge (texts, a building), one over the other.
	EXPECT_LT(together * 3, apart) << "drawn as one group, the order hardly matters";
}

// A navigational warning over the chart it concerns: the vector overlays of the System Database
// (S-124, S-128, ...) are drawn over the S-101 cells, the grids under them.
TEST(RenderSmoke, SystemDatabaseChartDrawsS124OverTheS101)
{
	const auto cell = testpaths::tdsRoot() / L"KOR_TDS" / L"S100_ROOT" / L"S-101" / L"DATASET_FILES" / L"101KR00659C72" / L"101KR00659C72.000";
	const auto warning = testpaths::korS124Gml();
	if (!std::filesystem::exists(cell) || !std::filesystem::exists(warning))
	{
		GTEST_SKIP() << "KOR S-101/S-124 test data not available under " << testpaths::tdsRoot().string();
	}
	// The same chart with and without the warning.
	const auto install = [&](const std::filesystem::path& root, bool withWarning) {
		std::filesystem::remove_all(root);
		GISLibrary::SystemDatabaseSession session(root);
		auto& importer = session.importer();
		ASSERT_EQ(importer.importFeatureCatalogue(testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml").outcome, s100::sdb::ImportOutcome::Added);
		ASSERT_EQ(importer.importPortrayalCatalogue(testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal").outcome, s100::sdb::ImportOutcome::Added);
		ASSERT_EQ(importer.importDataset(cell).outcome, s100::sdb::ImportOutcome::Added);
		if (withWarning)
		{
			ASSERT_EQ(importer.importFeatureCatalogue(testpaths::repoRoot() / "ProgramData" / "FC" / "S-124_FC.xml").outcome, s100::sdb::ImportOutcome::Added);
			ASSERT_EQ(importer.importPortrayalCatalogue(testpaths::repoRoot() / "ProgramData" / "PC" / "S124_Portrayal").outcome, s100::sdb::ImportOutcome::Added);
			ASSERT_EQ(importer.importDataset(warning).outcome, s100::sdb::ImportOutcome::Added);
		}
	};
	const auto chartOnly = testpaths::tempDir() / L"SystemDatabaseChartS101Only";
	const auto withWarning = testpaths::tempDir() / L"SystemDatabaseChartS101S124";
	install(chartOnly, false);
	install(withWarning, true);

	GdiplusSession gdiplus;
	const auto draw = [&](const std::filesystem::path& root, MemoryCanvas& canvas) {
		CGISLibraryApp gis;
		auto* catalogues = gis.GetCatalogManager();
		catalogues->onPortrayalLoaded = [&gis](PortrayalCatalogue* pc) { GISLibrary::PortrayalRender::Prepare(pc, gis.GetD2D1Resources()); };
		const RECT view{ 0, 0, kWidth, kHeight };
		gis.SetScreen(view);
		GISLibrary::SystemDatabaseSession session(root);
		session.attach(*gis.GetLayerManager(), *catalogues);
		// Around the warning area off Busan North Port, at the cell's scale.
		MBR area(129.045, 35.105, 129.063, 35.119);
		projection(area);
		gis.SetMap(area);
		gis.Draw(canvas.dc());
		::GdiFlush();
	};
	MemoryCanvas chart(kWidth, kHeight);
	MemoryCanvas both(kWidth, kHeight);
	draw(chartOnly, chart);
	draw(withWarning, both);
	EXPECT_TRUE(both.SavePng(OutputDirectory() / "sdb-chart-s101-s124.png"));
	std::size_t nonWhite = 0;
	std::size_t colours = 0;
	chart.Measure(nonWhite, colours);
	const std::size_t changed = chart.DifferingPixels(both);
	std::cout << "[ sdb chart ] S-101 non-white " << nonWhite << ", pixels the S-124 changes " << changed << std::endl;
	EXPECT_GT(nonWhite, 100000u) << "the chart is drawn";
	EXPECT_GT(changed, 1000u) << "the warning shows over the chart";
}
