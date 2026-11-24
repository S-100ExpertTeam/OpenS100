#pragma once
// The Skia backend of IRenderer (Documents/TechDocs/2026-11-19-r6-skia-backend-design.md).
//
// It draws on a canvas it is handed and knows nothing else of the surface: whether the pixels
// are in memory or on a GPU, and where they go afterwards, is the application's business.
//
// It draws paths, stroked (solid, dashed, arcs) and filled with a colour or a pattern, and texts
// in the fonts it is handed. The symbols, pattern fills and line styles of the catalogue are
// drawn by the shared code (SymbolPicture, PatternTile, LineStylePicture) as paths on this
// renderer: the backend does not know what they are. What it was asked for and had no resource
// to draw is counted (NotDrawn).
#include "../GISLibrary/DisplayListPainter.h"
#include "../GISLibrary/IRenderer.h"

#include "include/core/SkMatrix.h"
#include "include/core/SkRefCnt.h"

#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>

class SkCanvas;
class SkImage;
class SkPath;
class SkShader;

namespace s100::render
{
	class AreaFillLibrary;
	class LineStyleLibrary;
	class SkiaFonts;
	class SymbolLibrary;
	struct SymbolPicture;

	// The symbols of a catalogue as they come out on the screen, each drawn once and kept as an
	// image: a chart shows the same few symbols thousands of times a frame, and laying an image
	// down costs a fraction of drawing the shapes of a symbol again.
	//
	// An image is of one symbol at one size, turned one way, in the colours of the palette it
	// was drawn in: whoever keeps the images empties them (Clear) when the palette changes.
	class SkiaSymbolImages
	{
	public:
		SkiaSymbolImages();
		~SkiaSymbolImages();

		struct Image
		{
			sk_sp<SkImage> image;   // nothing: the symbol leaves no mark
			int left = 0;           // where the corner of the image is from the pivot, in pixels
			int top = 0;
		};

		// The image kept for a symbol, or nothing when it has not been drawn yet.
		const Image* Find(const SymbolPicture* picture, float rotationDegrees, float scale) const;
		const Image& Keep(const SymbolPicture* picture, float rotationDegrees, float scale, Image image);

		void Clear();

		size_t Size() const
		{
			return images.size();
		}

		// A turn is kept to a quarter of a degree, so symbols turned to all sides (to north about
		// a pole) still share images.
		static float RoundedTurn(float rotationDegrees);

	private:
		struct Key
		{
			const SymbolPicture* picture;
			std::int32_t quarterDegrees;
			float scale;

			bool operator==(const Key& other) const
			{
				return picture == other.picture && quarterDegrees == other.quarterDegrees && scale == other.scale;
			}
		};

		struct KeyHash
		{
			size_t operator()(const Key& key) const;
		};

		static Key KeyOf(const SymbolPicture* picture, float rotationDegrees, float scale);

		std::unordered_map<Key, Image, KeyHash> images;
	};

	// What the backend draws with besides the canvas. The application owns what it points to
	// and keeps it for as long as it draws.
	struct SkiaResources
	{
		// The symbols of the portrayal catalogue; without them no symbol is drawn.
		SymbolLibrary* symbols = nullptr;
		// The area fills of the catalogue, for the pattern fills that name one.
		const AreaFillLibrary* areaFills = nullptr;
		// The line styles of the catalogue.
		const LineStyleLibrary* lineStyles = nullptr;
		// The colours of the palette in use, for the symbols.
		ColorLookup color;
		// Where the symbols drawn are kept as images, in the colours of `color`; without it every
		// symbol is drawn from its shapes each time.
		SkiaSymbolImages* symbolImages = nullptr;
		// The fonts texts are written in; without them no text is drawn and every text
		// measures nothing.
		const SkiaFonts* fonts = nullptr;
	};

	class SkiaRenderer final : public IRenderer
	{
	public:
		// The canvas outlives the renderer. Its matrix is taken as device pixels: the renderer
		// draws under that matrix and leaves it as it found it.
		explicit SkiaRenderer(SkCanvas& canvas, SkiaResources resources = {});
		~SkiaRenderer() override;

		const void* Backend() const override;

		void SetTransform(const Transform& transform) override;
		void StrokePath(const Path& path, const ColorF& color, const Stroke& stroke) override;
		std::unique_ptr<RetainedPath> Retain(const Path& path) override;
		void StrokePath(const RetainedPath& path, const ColorF& color, const Stroke& stroke) override;
		void FillPath(const Path& path, const Fill& fill) override;
		void FillPath(const RetainedPath& path, const Fill& fill) override;
		std::unique_ptr<RetainedImage> RetainImage(const std::uint8_t* pixels, int width, int height) override;
		void DrawImage(const RetainedImage& image, const ImageRect& source, const ImageRect& target) override;
		void PushClip(const Path& path) override;
		void PopClip() override;
		void StrokeLineStyle(const std::string& reference, const std::vector<PointF>& points,
			const ColorF* shortStretches) override;
		void DrawSymbol(const std::string& reference, PointF at, float rotationDegrees, float scale) override;
		SizeF MeasureText(const std::string& text, const TextFont& font) override;
		void FillText(const std::string& text, PointF topLeft, const TextFont& font, const ColorF& color) override;

		// What the painter asked for and this backend does not draw yet.
		struct Omitted
		{
			int symbols = 0;
			int lineStyles = 0;
			int patternFills = 0;
			int texts = 0;
		};

		const Omitted& NotDrawn() const
		{
			return omitted;
		}

	private:
		void Stroke_(const SkPath& path, const ColorF& color, const Stroke& stroke);
		void Fill_(const SkPath& path, const Fill& fill);
		// The tile of a pattern fill, repeated over the surface; nothing when there is no
		// such pattern.
		sk_sp<SkShader> Pattern(const Fill& fill);
		// Lays the kept image of a symbol down with its pivot at a point. False when the symbol
		// is to be drawn from its shapes (too large to keep, or a canvas that is scaled or turned).
		bool DrawSymbolImage(const SymbolPicture& picture, PointF at, float rotationDegrees, float scale);

		SkCanvas& canvas;
		SkiaResources resources;
		SkMatrix matrix;   // the transform of the painter, onto device pixels
		// The patterns made so far, by what the fill asked for. They are in the colours of the
		// palette this renderer was made with.
		std::map<std::string, sk_sp<SkShader>> patterns;
		Omitted omitted;
	};
}
