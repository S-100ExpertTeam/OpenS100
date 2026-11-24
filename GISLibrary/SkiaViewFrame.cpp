#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include "SkiaViewFrame.h"

#include "DisplayListPainter.h"
#include "DrawingOrder.h"
#include "LineStylePicture.h"
#include "PatternTile.h"
#include "PortrayalRenderCache.h"
#include "SymbolPicture.h"

#include "../GeoMetryLibrary/ENCCommon.h"
#include "../GeoMetryLibrary/Scaler.h"
#include "../RenderSkia/SkiaFonts.h"
#include "../RenderSkia/SkiaRenderer.h"

#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkSurface.h"
#include "include/core/SkSurfaceProps.h"

#include <cstdlib>
#include <map>
#include <string>

#pragma comment(lib, "msimg32.lib")   // AlphaBlend

namespace
{
	// Skia's native 32-bit pixels are what a Windows bitmap holds: blue first, premultiplied.
	static_assert(kN32_SkColorType == kBGRA_8888_SkColorType, "the layer is blended as a BGRA bitmap");

	bool SkiaFromEnvironment()
	{
		char* value = nullptr;
		size_t length = 0;
		bool skia = false;
		if (_dupenv_s(&value, &length, "S100_RENDER_BACKEND") == 0 && value)
		{
			skia = _stricmp(value, "skia") == 0;
			free(value);
		}
		return skia;
	}

	bool& SkiaSwitch()
	{
		static bool skia = SkiaFromEnvironment();
		return skia;
	}

	// What a catalogue's instructions are drawn with, read once.
	struct CatalogueResources
	{
		explicit CatalogueResources(PortrayalCatalogue& catalogue)
			: symbols(catalogue), areaFills(catalogue), lineStyles(catalogue)
		{
		}

		s100::render::SymbolLibrary symbols;
		s100::render::AreaFillLibrary areaFills;
		s100::render::LineStyleLibrary lineStyles;
	};
}

namespace GISLibrary
{
	bool DrawsWithSkia()
	{
		return SkiaSwitch();
	}

	void DrawWithSkia(bool on)
	{
		SkiaSwitch() = on;
	}

	struct SkiaViewFrame::State
	{
		// The pixels: a bitmap of the view's size that both GDI and Skia know.
		HDC memory = nullptr;
		HBITMAP bitmap = nullptr;
		HGDIOBJ previous = nullptr;
		void* bits = nullptr;
		int width = 0;
		int height = 0;
		RECT view{};
		sk_sp<SkSurface> surface;

		// The fonts, of the display font the viewer is set to.
		std::string fontName;
		std::unique_ptr<s100::render::SkiaFonts> fonts;

		std::map<PortrayalCatalogue*, std::unique_ptr<CatalogueResources>> catalogues;

		// The renderers of the frame in hand, one a catalogue: a renderer keeps the pattern
		// tiles it has made, which are of the palette of the frame.
		std::map<PortrayalCatalogue*, std::unique_ptr<s100::render::SkiaRenderer>> renderers;

		~State()
		{
			Release();
		}

		void Release()
		{
			renderers.clear();
			surface.reset();
			if (memory)
			{
				::SelectObject(memory, previous);
				::DeleteDC(memory);
				memory = nullptr;
			}
			if (bitmap)
			{
				::DeleteObject(bitmap);
				bitmap = nullptr;
			}
			bits = nullptr;
			width = 0;
			height = 0;
		}

		bool Size(int newWidth, int newHeight)
		{
			if (surface && newWidth == width && newHeight == height)
			{
				return true;
			}
			Release();
			if (newWidth <= 0 || newHeight <= 0)
			{
				return false;
			}

			BITMAPINFO info{};
			info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			info.bmiHeader.biWidth = newWidth;
			info.bmiHeader.biHeight = -newHeight;   // top row first, as Skia has it
			info.bmiHeader.biPlanes = 1;
			info.bmiHeader.biBitCount = 32;
			info.bmiHeader.biCompression = BI_RGB;
			memory = ::CreateCompatibleDC(nullptr);
			bitmap = memory ? ::CreateDIBSection(memory, &info, DIB_RGB_COLORS, &bits, nullptr, 0) : nullptr;
			if (!bitmap || !bits)
			{
				Release();
				return false;
			}
			previous = ::SelectObject(memory, bitmap);

			// Texts at full contrast, as SkiaImage sets them.
			const SkSurfaceProps props(0, kUnknown_SkPixelGeometry, SkSurfaceProps::kMaxContrastInclusive, 2.2f);
			surface = SkSurfaces::WrapPixels(SkImageInfo::MakeN32Premul(newWidth, newHeight, nullptr), bits,
				static_cast<size_t>(newWidth) * 4, &props);
			if (!surface)
			{
				Release();
				return false;
			}
			width = newWidth;
			height = newHeight;
			return true;
		}

		void Fonts()
		{
			const std::string& name = ENCCommon::DISPLAY_FONT_NAME;
			if (fonts && name == fontName)
			{
				return;
			}
			renderers.clear();
			fonts = std::make_unique<s100::render::SkiaFonts>();
			fontName = name;
			// The font the viewer is set to, then fonts every Windows has for what it lacks.
			fonts->AddSystem(name);
			for (const char* fallback : { "Malgun Gothic", "Segoe UI", "Segoe UI Symbol" })
			{
				if (name != fallback)
				{
					fonts->AddSystem(fallback);
				}
			}
		}
	};

	SkiaViewFrame& SkiaViewFrame::Shared()
	{
		static SkiaViewFrame frame;
		return frame;
	}

	SkiaViewFrame::SkiaViewFrame() : state(std::make_unique<State>())
	{
	}

	SkiaViewFrame::~SkiaViewFrame() = default;

	bool SkiaViewFrame::Begin(const Scaler& scaler)
	{
		const auto screen = scaler.GetScreenRect();
		state->view = RECT{ screen.left, screen.top, screen.right, screen.bottom };
		state->renderers.clear();
		if (!state->Size(screen.right - screen.left, screen.bottom - screen.top))
		{
			return false;
		}
		state->Fonts();
		state->surface->getCanvas()->clear(SK_ColorTRANSPARENT);
		return true;
	}

	void SkiaViewFrame::Paint(int priority, DrawingSet& drawingSet, PortrayalCatalogue* catalogue, Scaler& scaler, bool texts)
	{
		if (!state->surface || !catalogue)
		{
			return;
		}
		const s100::render::ColorLookup color = [catalogue](const std::wstring& token) {
			return PortrayalRender::Color(catalogue, token);
		};

		auto& renderer = state->renderers[catalogue];
		if (!renderer)
		{
			auto& resources = state->catalogues[catalogue];
			if (!resources)
			{
				resources = std::make_unique<CatalogueResources>(*catalogue);
			}
			renderer = std::make_unique<s100::render::SkiaRenderer>(*state->surface->getCanvas(),
				s100::render::SkiaResources{ &resources->symbols, &resources->areaFills, &resources->lineStyles, color, state->fonts.get() });
		}

		s100::render::DisplayListPainter painter(*renderer, scaler, color);
		s100::render::PaintPriority(priority, drawingSet, painter, texts);
	}

	void SkiaViewFrame::End(HDC target)
	{
		state->renderers.clear();
		if (!state->surface || !target)
		{
			return;
		}
		const BLENDFUNCTION blend{ AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		::AlphaBlend(target, state->view.left, state->view.top, state->width, state->height,
			state->memory, 0, 0, state->width, state->height, blend);
	}

	void SkiaViewFrame::ForgetCatalogues()
	{
		state->renderers.clear();
		state->catalogues.clear();
	}
}
