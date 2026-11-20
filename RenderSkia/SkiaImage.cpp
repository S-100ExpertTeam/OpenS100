#include "SkiaImage.h"

#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkPixmap.h"
#include "include/core/SkStream.h"
#include "include/core/SkSurface.h"
#include "include/encode/SkPngEncoder.h"

#include <fstream>
#include <vector>

namespace s100::render
{
	SkiaImage::SkiaImage(int width, int height, const GISLibrary::ColorF& background)
	{
		surface = SkSurfaces::Raster(SkImageInfo::MakeN32Premul(width, height, nullptr));
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
