#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include "SkiaViewFrame.h"

#include "CoverageDataset.h"
#include "DepthMaskStages.h"
#include "DisplayListPainter.h"
#include "DrawingOrder.h"
#include "GISLibrary.h"
#include "LineStylePicture.h"
#include "PatternTile.h"
#include "PortrayalRenderCache.h"
#include "SymbolPicture.h"

#include "../GeoMetryLibrary/ENCCommon.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"
#include "../GeoMetryLibrary/Scaler.h"
#include "../RenderSkia/SkiaFonts.h"
#include "../RenderSkia/SkiaImage.h"
#include "../RenderSkia/SkiaRenderer.h"

#include "include/core/SkBBHFactory.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkImage.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPicture.h"
#include "include/core/SkPictureRecorder.h"
#include "include/core/SkPixmap.h"
#include "include/core/SkSamplingOptions.h"
#include "include/core/SkSurface.h"
#include "include/core/SkSurfaceProps.h"

#include <algorithm>
#include <array>
#include <condition_variable>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#pragma comment(lib, "msimg32.lib")   // AlphaBlend
#endif

namespace
{
#ifdef _WIN32
	// Skia's native 32-bit pixels are what a Windows bitmap holds: blue first, premultiplied.
	static_assert(kN32_SkColorType == kBGRA_8888_SkColorType, "the layer is blended as a BGRA bitmap");
#endif

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

		// The symbols drawn so far as images, in the colours of the palette named.
		s100::render::SkiaSymbolImages symbolImages;
		std::string symbolPalette;
	};

	// The threads a frame is rasterised on besides the one that draws it. They are started with
	// the first frame that has work for them and wait between frames.
	class BandWorkers
	{
	public:
		explicit BandWorkers(int count)
		{
			for (int i = 0; i < count; i++)
			{
				threads.emplace_back([this]() { Work(); });
			}
		}

		~BandWorkers()
		{
			{
				const std::lock_guard<std::mutex> lock(mutex);
				stopping = true;
			}
			wake.notify_all();
			for (std::thread& thread : threads)
			{
				thread.join();
			}
		}

		BandWorkers(const BandWorkers&) = delete;
		BandWorkers& operator=(const BandWorkers&) = delete;

		// Calls job(0) ... job(count - 1), each once, on the workers and on the calling thread,
		// and comes back when every call has returned.
		void Run(int count, const std::function<void(int)>& job)
		{
			if (count <= 0)
			{
				return;
			}
			{
				const std::lock_guard<std::mutex> lock(mutex);
				current = &job;
				total = count;
				next = 0;
				unfinished = count;
				round++;
			}
			wake.notify_all();
			Take(job, count);

			std::unique_lock<std::mutex> lock(mutex);
			done.wait(lock, [this]() { return unfinished == 0; });
			current = nullptr;
		}

	private:
		// Takes the calls that are left, one at a time, until there is none.
		void Take(const std::function<void(int)>& job, int count)
		{
			for (;;)
			{
				int index = 0;
				{
					const std::lock_guard<std::mutex> lock(mutex);
					if (current != &job || next >= count)
					{
						return;
					}
					index = next++;
				}
				job(index);
				{
					const std::lock_guard<std::mutex> lock(mutex);
					if (--unfinished == 0)
					{
						done.notify_all();
					}
				}
			}
		}

		void Work()
		{
			std::uint64_t seen = 0;
			for (;;)
			{
				const std::function<void(int)>* job = nullptr;
				int count = 0;
				{
					std::unique_lock<std::mutex> lock(mutex);
					wake.wait(lock, [&]() { return stopping || (round != seen && current); });
					if (stopping)
					{
						return;
					}
					seen = round;
					job = current;
					count = total;
				}
				Take(*job, count);
			}
		}

		std::vector<std::thread> threads;
		std::mutex mutex;
		std::condition_variable wake;
		std::condition_variable done;
		const std::function<void(int)>* current = nullptr;   // the job of the round in hand
		int total = 0;
		int next = 0;          // the next call nobody has taken
		int unfinished = 0;    // calls not yet returned
		std::uint64_t round = 0;
		bool stopping = false;
	};

	// A frame with fewer drawing calls than this is rasterised by the thread that drew it:
	// handing it round would cost more than it saves.
	constexpr int bandMinCalls = 512;

	// No more threads than this rasterise a frame, however many the machine has.
	constexpr int bandMaxThreads = 8;
}

namespace GISLibrary
{
	struct SkiaViewFrame::State
	{
		// The pixels of the view's size. On Windows a bitmap that both GDI and Skia know.
#ifdef _WIN32
		HDC memory = nullptr;
		HBITMAP bitmap = nullptr;
		HGDIOBJ previous = nullptr;
#else
		std::vector<std::uint32_t> owned;
#endif
		void* bits = nullptr;
		int width = 0;
		int height = 0;
		// The view on its device: left, top, right, bottom.
		std::array<long, 4> view{};
		bool drawn = false;              // the pixels hold a frame
		sk_sp<SkSurface> surface;

		// What is drawn in a frame is written down (a recording) and rasterised when the frame
		// ends: the surface is cut into bands, each a surface over its own rows of the same
		// pixels, and several threads play the recording back onto a band each. A band draws
		// what touches it and nothing else, so no two threads write a pixel.
		struct Band
		{
			sk_sp<SkSurface> surface;
			int top = 0;
		};
		std::vector<Band> bands;
		SkPictureRecorder recorder;
		SkCanvas* recording = nullptr;   // the canvas of the frame in hand; null outside a frame
		std::unique_ptr<BandWorkers> workers;
		int threads = 1;                 // the threads a frame is rasterised on, this one counted

		// The scene in hand: how many are open, the device context of the outermost, and whether
		// frames ended on it wait in the recording to be shown.
		int scenes = 0;
		HDC sceneTarget = nullptr;
		bool sceneWaiting = false;
		int frameSave = 0;               // the save count of the recording when the frame began

		// The fonts, of the display font the viewer is set to.
		std::string fontName;
		std::vector<std::filesystem::path> fontFiles;
		bool fontFilesChanged = false;
		std::unique_ptr<s100::render::SkiaFonts> fonts;

		std::map<PortrayalCatalogue*, std::unique_ptr<CatalogueResources>> catalogues;

		// The outlines of the S-102 data that hides the S-101 depth features (S-98 D-1), in
		// the coordinates of the datasets; made again when the outlines may have changed.
		s100::render::WorldRings depthMask;
		std::uint64_t depthMaskVersion = ~std::uint64_t{ 0 };

		const s100::render::WorldRings& DepthMask()
		{
			const std::uint64_t version = CoverageDataset::SuppressionVersion();
			if (version != depthMaskVersion)
			{
				depthMaskVersion = version;
				depthMask.clear();
				for (const auto& ring : CoverageDataset::SuppressionRings())
				{
					std::vector<s100::render::WorldPoint> points;
					points.reserve(ring.size());
					for (const auto& position : ring)
					{
						double x = position.lon;
						double y = position.lat;
						projection(x, y);
						points.push_back({ x, y });
					}
					depthMask.push_back(std::move(points));
				}
			}
			return depthMask;
		}

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
			if (recording)
			{
				recording = nullptr;
				recorder.finishRecordingAsPicture();
			}
			bands.clear();
			surface.reset();
#ifdef _WIN32
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
#else
			owned.clear();
#endif
			bits = nullptr;
			drawn = false;
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

#ifdef _WIN32
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
#else
			owned.assign(static_cast<size_t>(newWidth) * static_cast<size_t>(newHeight), 0);
			bits = owned.data();
#endif

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

			// Two bands a thread, so a thread that gets an empty band has another to take.
			threads = std::clamp(static_cast<int>(std::thread::hardware_concurrency()), 1, bandMaxThreads);
			const int count = threads > 1 ? std::min(threads * 2, newHeight) : 0;
			for (int i = 0; i < count; i++)
			{
				Band band;
				band.top = static_cast<int>(static_cast<std::int64_t>(newHeight) * i / count);
				const int bottom = static_cast<int>(static_cast<std::int64_t>(newHeight) * (i + 1) / count);
				band.surface = SkSurfaces::WrapPixels(SkImageInfo::MakeN32Premul(newWidth, bottom - band.top, nullptr),
					static_cast<std::uint8_t*>(bits) + static_cast<size_t>(band.top) * newWidth * 4,
					static_cast<size_t>(newWidth) * 4, &props);
				if (!band.surface)
				{
					bands.clear();   // the frame is rasterised whole
					break;
				}
				bands.push_back(std::move(band));
			}
			return true;
		}

		// Puts what the frame recorded on the pixels.
		// False when the frame drew nothing: there is nothing to put on the screen then.
		bool Rasterise()
		{
			if (!recording)
			{
				return false;
			}
			recording = nullptr;
			const sk_sp<SkPicture> recorded = recorder.finishRecordingAsPicture();
			if (!recorded || !surface || recorded->approximateOpCount() == 0)
			{
				return false;
			}

			drawn = true;
			if (bands.empty() || recorded->approximateOpCount() < bandMinCalls)
			{
				SkCanvas* canvas = surface->getCanvas();
				canvas->clear(SK_ColorTRANSPARENT);
				canvas->drawPicture(recorded);
				return true;
			}

			if (!workers)
			{
				workers = std::make_unique<BandWorkers>(threads - 1);
			}
			const SkPicture* picture = recorded.get();
			workers->Run(static_cast<int>(bands.size()), [this, picture](int index) {
				const Band& band = bands[static_cast<size_t>(index)];
				SkCanvas* canvas = band.surface->getCanvas();
				canvas->clear(SK_ColorTRANSPARENT);
				canvas->save();
				canvas->translate(0.0f, static_cast<float>(-band.top));
				canvas->drawPicture(picture);
				canvas->restore();
			});
			return true;
		}

		void Fonts()
		{
			const std::string& name = ENCCommon::DISPLAY_FONT_NAME;
			if (fonts && name == fontName && !fontFilesChanged)
			{
				return;
			}
			renderers.clear();
			fonts = std::make_unique<s100::render::SkiaFonts>();
			fontName = name;
			fontFilesChanged = false;
			// The files the application handed over come first; with them a text is the same on
			// every system.
			for (const auto& file : fontFiles)
			{
				fonts->Add(file);
			}
			if (!fontFiles.empty())
			{
				return;
			}
			// The font the viewer is set to, then fonts every Windows has for what it lacks. Where
			// the system cannot be asked for a font by name these find none.
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
		const std::array<long, 4> view{ screen.left, screen.top, screen.right, screen.bottom };
		state->renderers.clear();

		// A frame of a scene goes on drawing where the frames before it left off, as long as the
		// view is the same.
		if (state->sceneWaiting && state->recording)
		{
			if (view == state->view)
			{
				state->Fonts();
				state->frameSave = state->recording->save();
				return true;
			}
			Show(state->sceneTarget);
		}

		state->view = view;
		if (!state->Size(screen.right - screen.left, screen.bottom - screen.top))
		{
			return false;
		}
		state->Fonts();
		// What the frame draws is written down with where each call lands, so a band plays back
		// only the calls that touch it.
		if (state->recording)
		{
			// A frame begun and never ended: what it recorded is dropped.
			state->recording = nullptr;
			state->recorder.finishRecordingAsPicture();
		}
		SkRTreeFactory placed;
		state->recording = state->recorder.beginRecording(
			SkRect::MakeWH(static_cast<float>(state->width), static_cast<float>(state->height)), &placed);
		if (!state->recording)
		{
			return false;
		}
		state->frameSave = state->recording->save();
		return true;
	}

	s100::render::IRenderer* SkiaViewFrame::Renderer(PortrayalCatalogue* catalogue)
	{
		if (!state->surface || !state->recording)
		{
			return nullptr;
		}
		auto& renderer = state->renderers[catalogue];
		if (!renderer)
		{
			s100::render::SkiaResources with;
			with.fonts = state->fonts.get();
			if (catalogue)
			{
				auto& resources = state->catalogues[catalogue];
				if (!resources)
				{
					resources = std::make_unique<CatalogueResources>(*catalogue);
				}
				// The images of the symbols are in the colours of a palette: another, and they go.
				const std::string palette = catalogue->GetCurrentPaletteName();
				if (palette != resources->symbolPalette)
				{
					resources->symbolImages.Clear();
					resources->symbolPalette = palette;
				}
				with.symbolImages = &resources->symbolImages;
				with.symbols = &resources->symbols;
				with.areaFills = &resources->areaFills;
				with.lineStyles = &resources->lineStyles;
				with.color = [catalogue](const std::wstring& token) { return PortrayalRender::Color(catalogue, token); };
			}
			renderer = std::make_unique<s100::render::SkiaRenderer>(*state->recording, with);
		}
		return renderer.get();
	}

	void SkiaViewFrame::Paint(int priority, DrawingSet& drawingSet, PortrayalCatalogue* catalogue, Scaler& scaler, bool texts)
	{
		s100::render::IRenderer* renderer = catalogue ? Renderer(catalogue) : nullptr;
		if (!renderer)
		{
			return;
		}
		const s100::render::ColorLookup color = [catalogue](const std::wstring& token) {
			return PortrayalRender::Color(catalogue, token);
		};

		s100::render::DisplayListPainter painter(*renderer, scaler, color);
		s100::render::DepthMaskStages stages(*renderer, scaler, state->DepthMask());
		s100::render::PaintPriority(priority, drawingSet, painter, texts, &stages);
	}

	SkiaViewFrame::Scene::Scene(SkiaViewFrame& frame) : Scene(frame, nullptr)
	{
	}

	SkiaViewFrame::Scene::Scene(SkiaViewFrame& frame, HDC target) : frame(frame)
	{
		if (frame.state->scenes++ == 0)
		{
			frame.state->sceneTarget = target;
		}
	}

	SkiaViewFrame::Scene::~Scene()
	{
		State& held = *frame.state;
		if (--held.scenes == 0)
		{
			if (held.sceneWaiting)
			{
				frame.Show(held.sceneTarget);
			}
			held.sceneTarget = nullptr;
		}
	}

	void SkiaViewFrame::End()
	{
		End(state->sceneTarget);
	}

	void SkiaViewFrame::End(HDC target)
	{
		state->renderers.clear();
		if (!state->recording)
		{
			return;   // no frame was begun
		}
		// What the frame left set (a clip, a transform of the canvas) ends with it.
		state->recording->restoreToCount(state->frameSave);

		if (state->scenes > 0 && target == state->sceneTarget)
		{
			state->sceneWaiting = true;   // shown with the rest of the scene
			return;
		}
		Show(target);
	}

	void SkiaViewFrame::Show(HDC target)
	{
		state->sceneWaiting = false;
		if (!state->Rasterise() || !target)
		{
			return;
		}
#ifdef _WIN32
		const BLENDFUNCTION blend{ AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		::AlphaBlend(target, state->view[0], state->view[1], state->width, state->height,
			state->memory, 0, 0, state->width, state->height, blend);
#endif
	}

	void SkiaViewFrame::SetFontFiles(std::vector<std::filesystem::path> files)
	{
		state->fontFiles = std::move(files);
		state->fontFilesChanged = true;
	}

	bool SkiaViewFrame::WritePng(const std::filesystem::path& path, const ColorF& background) const
	{
		if (!state->bits || state->width <= 0 || state->height <= 0)
		{
			return false;
		}
		s100::render::SkiaImage image(state->width, state->height, background);
		if (!image.Valid())
		{
			return false;
		}
		// A frame that drew nothing left the pixels as they were: the picture is the background.
		if (state->drawn)
		{
			const SkPixmap pixels(SkImageInfo::MakeN32Premul(state->width, state->height, nullptr), state->bits,
				static_cast<size_t>(state->width) * 4);
			const sk_sp<SkImage> layer = SkImages::RasterFromPixmapCopy(pixels);
			if (!layer)
			{
				return false;
			}
			image.Canvas().drawImage(layer, 0.0f, 0.0f, SkSamplingOptions(), nullptr);
		}
		return image.WritePng(path);
	}

	void SkiaViewFrame::Draw(CGISLibraryApp& view, int offset)
	{
		state->drawn = false;
		const Scene scene(*this);
		view.Draw(*this, offset);
	}

	void SkiaViewFrame::ForgetCatalogues()
	{
		state->renderers.clear();
		state->catalogues.clear();
	}
}

#ifdef _WIN32
// Everything a refresh draws is one scene: the layers are rasterised together and put on the
// device context once.
void CGISLibraryApp::Draw(HDC& hDC, int offset)
{
	auto& frame = GISLibrary::SkiaViewFrame::Shared();
	const GISLibrary::SkiaViewFrame::Scene scene(frame, hDC);
	Draw(frame, offset);
}
#endif
