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

#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <vector>

#include "TestPaths.h"
#include "GISLibrary.h"
#include "PortrayalRenderCache.h"

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
		GdiplusSession gdiplus;
		CGISLibraryApp gis;

		auto cm = gis.GetCatalogManager();
		ASSERT_NE(cm->addFC((testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml").string()), nullptr);
		auto pc = cm->addPC((testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml").wstring());
		ASSERT_NE(pc, nullptr);
		GISLibrary::PortrayalRender::Prepare(pc, gis.GetD2D1Resources());

		const RECT view{ 0, 0, kWidth, kHeight };
		gis.SetScreen(view);

		ASSERT_TRUE(gis.AddLayer(cell.wstring())) << cell.string();
		gis.SetViewMBR(view);

		MemoryCanvas canvas(kWidth, kHeight);
		ASSERT_TRUE(canvas.valid());
		gis.Draw(canvas.dc());

		const auto png = OutputDirectory() / (cell.stem().string() + ".png");
		EXPECT_TRUE(canvas.SavePng(png)) << png.string();
		canvas.Measure(nonWhite, colours);
		std::cout << "[ render   ] " << png.string() << "  non-white pixels: " << nonWhite
			<< ", distinct colours: " << colours << std::endl;
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
