#pragma once
// A picture in memory for the Skia backend to draw on with no window and no GPU, and its PNG
// file. An application with a surface of its own (a window, a GPU context) does not need this.
#include "../GISLibrary/DrawTypes.h"

#include "include/core/SkRefCnt.h"

#include <filesystem>

class SkCanvas;
class SkSurface;

namespace s100::render
{
	class SkiaImage
	{
	public:
		// 32-bit pixels, premultiplied, filled with a colour. No colour space is set: sRGB
		// values are blended as they are, as the viewer has always done.
		SkiaImage(int width, int height, const GISLibrary::ColorF& background);
		~SkiaImage();

		SkiaImage(const SkiaImage&) = delete;
		SkiaImage& operator=(const SkiaImage&) = delete;

		// False when the pixels could not be allocated; there is no canvas then.
		bool Valid() const;
		SkCanvas& Canvas();

		// Every pixel to one colour: the start of another frame.
		void Clear(const GISLibrary::ColorF& color);

		bool WritePng(const std::filesystem::path& path) const;

	private:
		sk_sp<SkSurface> surface;
	};
}
