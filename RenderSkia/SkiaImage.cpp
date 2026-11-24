#include "SkiaImage.h"

#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkPixmap.h"
#include "include/core/SkStream.h"
#include "include/core/SkSurface.h"
#include "include/core/SkSurfaceProps.h"
#include "include/encode/SkPngEncoder.h"

#include <fstream>
#include <vector>

namespace s100::render
{
	SkiaImage::SkiaImage(int width, int height, const GISLibrary::ColorF& background)
	{
		// Texts at full contrast: thin strokes of small glyphs come out as dark as a display
		// with DirectWrite shows them, not as a grey haze.
		const SkSurfaceProps props(0, kUnknown_SkPixelGeometry, SkSurfaceProps::kMaxContrastInclusive, 2.2f);
		surface = SkSurfaces::Raster(SkImageInfo::MakeN32Premul(width, height, nullptr), &props);
		if (surface)
		{
			surface->getCanvas()->clear(SkColor4f{ background.r, background.g, background.b, background.a });
		}
	}

	SkiaImage::~SkiaImage() = default;

	bool SkiaImage::Valid() const
	{
		return surface != nullptr;
	}

	SkCanvas& SkiaImage::Canvas()
	{
		return *surface->getCanvas();
	}

	void SkiaImage::Clear(const GISLibrary::ColorF& color)
	{
		if (surface)
		{
			surface->getCanvas()->clear(SkColor4f{ color.r, color.g, color.b, color.a });
		}
	}

	bool SkiaImage::WritePng(const std::filesystem::path& path) const
	{
		SkPixmap pixels;
		if (!surface || !surface->peekPixels(&pixels))
		{
			return false;
		}
		// Encoded in memory and written by the standard library, which takes a path as it is on
		// every platform.
		SkDynamicMemoryWStream encoded;
		if (!SkPngEncoder::Encode(&encoded, pixels, SkPngEncoder::Options()))
		{
			return false;
		}
		std::vector<char> bytes(encoded.bytesWritten());
		encoded.copyTo(bytes.data());

		std::ofstream out(path, std::ios::binary | std::ios::trunc);
		out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
		return out.good();
	}
}
