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
#include <iostream>
#include <numeric>
#include <vector>

#include "TestPaths.h"
#include "GISLibrary.h"
#include "PortrayalRenderCache.h"
#include "S100ExchangeCatalogue.h"
#include "CoverageDataset.h"
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
	CoverageDataset::SetDepthSettings(original);

	EXPECT_GT(status.applied, 0u);
	EXPECT_TRUE(status.issues.contains(s100::coverage::WlaIssue::IncompatibleDatums));
	const auto changed = std::inner_product(plain.begin(), plain.end(), adjusted.begin(), std::size_t{ 0 }, std::plus<>(),
		[](std::uint32_t a, std::uint32_t b) { return a != b ? std::size_t{ 1 } : std::size_t{ 0 }; });
	std::cout << "[ wla ] pixels changed by WLA: " << changed << std::endl;
	EXPECT_GT(changed, 0u) << "adjusted depths draw differently (shading, contour, WLA boundary)";
}
