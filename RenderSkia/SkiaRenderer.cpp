#include "SkiaRenderer.h"
#include "SkiaText.h"

#include "../GISLibrary/LineStylePicture.h"
#include "../GISLibrary/PatternTile.h"
#include "../GISLibrary/SymbolPicture.h"

#include "include/core/SkBBHFactory.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPath.h"
#include "include/core/SkPathBuilder.h"
#include "include/core/SkPathEffect.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkImage.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkPathTypes.h"
#include "include/core/SkPixmap.h"
#include "include/core/SkSamplingOptions.h"
#include "include/core/SkPicture.h"
#include "include/core/SkPictureRecorder.h"
#include "include/core/SkSamplingOptions.h"
#include "include/core/SkShader.h"
#include "include/core/SkSurface.h"
#include "include/core/SkSurfaceProps.h"
#include "include/core/SkTileMode.h"
#include "include/effects/SkDashPathEffect.h"

#include <cmath>
#include <functional>

namespace
{
	using namespace s100::render;

	// A path kept as a Skia path, in the coordinates it was given in.
	struct RetainedSkImage : RetainedImage
	{
		sk_sp<SkImage> image;
	};

	struct RetainedSkPath : RetainedPath
	{
		SkPath path;
	};

	SkColor4f ToSk(const ColorF& color)
	{
		return { color.r, color.g, color.b, color.a };
	}

	// Where the point straight down the screen, (0, radius), lands when turned clockwise.
	SkPoint Turned(float radius, float degrees)
	{
		const double radians = static_cast<double>(degrees) * 3.14159265358979323846 / 180.0;
		return { static_cast<float>(-radius * std::sin(radians)), static_cast<float>(radius * std::cos(radians)) };
	}

	// A figure with no line (a single point) draws nothing and is left out. A fill has no arcs.
	SkPath Build(const Path& path, bool withArcs)
	{
		SkPathBuilder builder(SkPathFillType::kEvenOdd);
		for (const auto& figure : path.figures)
		{
			if (figure.lines.empty())
			{
				continue;
			}
			builder.moveTo(figure.start.x, figure.start.y);
			for (const auto& to : figure.lines)
			{
				builder.lineTo(to.x, to.y);
			}
			if (figure.closed)
			{
				builder.close();
			}
		}
		if (withArcs)
		{
			for (const auto& arc : path.arcs)
			{
				// A whole turn or more is the circle (the sector of an all-round light). Between
				// two points that are the same there is no arc to pick, and Direct2D draws
				// nothing there.
				if (std::fabs(arc.endDegrees - arc.startDegrees) >= 360.0f)
				{
					builder.addCircle(0.0f, 0.0f, arc.radius);
					continue;
				}
				// The arc of the circle about the origin that runs clockwise from one point to
				// the other: the large one when the angles are more than half a turn apart.
				const SkPoint start = Turned(arc.radius, arc.startDegrees);
				const SkPoint end = Turned(arc.radius, arc.endDegrees);
				builder.moveTo(start);
				builder.arcTo({ arc.radius, arc.radius }, 0.0f,
					(arc.endDegrees - arc.startDegrees) > 180 ? SkPathBuilder::kLarge_ArcSize : SkPathBuilder::kSmall_ArcSize,
					SkPathDirection::kCW, end);
			}
		}
		return builder.detach();
	}
}

namespace s100::render
{
	namespace
	{
		// Past this many images the kept ones are let go and made again as they are asked for:
		// a chart has a few hundred symbols, and only symbols turned to every side come near it.
		constexpr size_t symbolImageLimit = 4096;

		// A symbol larger than this a side is drawn from its shapes every time.
		constexpr int symbolImageMaxSide = 512;
	}

	SkiaSymbolImages::SkiaSymbolImages() = default;

	SkiaSymbolImages::~SkiaSymbolImages() = default;

	float SkiaSymbolImages::RoundedTurn(float rotationDegrees)
	{
		return std::round(rotationDegrees * 4.0f) / 4.0f;
	}

	SkiaSymbolImages::Key SkiaSymbolImages::KeyOf(const SymbolPicture* picture, float rotationDegrees, float scale)
	{
		// A whole turn more or less is the same picture.
		const float turn = std::fmod(rotationDegrees, 360.0f);
		std::int32_t quarters = static_cast<std::int32_t>(std::lround(turn * 4.0f)) % 1440;
		if (quarters < 0)
		{
			quarters += 1440;
		}
		return { picture, quarters, scale };
	}

	size_t SkiaSymbolImages::KeyHash::operator()(const Key& key) const
	{
		size_t hash = std::hash<const void*>()(key.picture);
		hash ^= std::hash<std::int32_t>()(key.quarterDegrees) + 0x9e3779b97f4a7c15ull + (hash << 6) + (hash >> 2);
		hash ^= std::hash<float>()(key.scale) + 0x9e3779b97f4a7c15ull + (hash << 6) + (hash >> 2);
		return hash;
	}

	const SkiaSymbolImages::Image* SkiaSymbolImages::Find(const SymbolPicture* picture, float rotationDegrees, float scale) const
	{
		const auto found = images.find(KeyOf(picture, rotationDegrees, scale));
		return found == images.end() ? nullptr : &found->second;
	}

	const SkiaSymbolImages::Image& SkiaSymbolImages::Keep(const SymbolPicture* picture, float rotationDegrees, float scale, Image image)
	{
		if (images.size() >= symbolImageLimit)
		{
			images.clear();
		}
		return images.insert_or_assign(KeyOf(picture, rotationDegrees, scale), std::move(image)).first->second;
	}

	void SkiaSymbolImages::Clear()
	{
		images.clear();
	}

	SkiaRenderer::SkiaRenderer(SkCanvas& canvas, SkiaResources resources) : canvas(canvas), resources(std::move(resources))
	{
	}

	SkiaRenderer::~SkiaRenderer() = default;

	const void* SkiaRenderer::Backend() const
	{
		static const char skia = 0;
		return &skia;
	}

	void SkiaRenderer::SetTransform(const Transform& transform)
	{
		matrix.reset();
		for (const auto& step : transform)
		{
			switch (step.kind)
			{
			case TransformStep::Kind::Rotate:
				matrix.postRotate(step.degrees, step.x, step.y);
				break;
			case TransformStep::Kind::Scale:
				matrix.postScale(step.factor, step.factor);
				break;
			default:
				matrix.postTranslate(step.x, step.y);
				break;
			}
		}
	}

	void SkiaRenderer::Stroke_(const SkPath& path, const ColorF& color, const Stroke& stroke)
	{
		SkPaint paint;
		paint.setAntiAlias(true);
		paint.setStyle(SkPaint::kStroke_Style);
		paint.setColor4f(ToSk(color), nullptr);
		paint.setStrokeWidth(stroke.width);

		if (stroke.dashes.empty())
		{
			paint.setStrokeCap(stroke.flatCaps ? SkPaint::kButt_Cap : SkPaint::kRound_Cap);
			paint.setStrokeJoin(stroke.flatCaps ? SkPaint::kMiter_Join : SkPaint::kRound_Join);
			paint.setStrokeMiter(10.0f);
		}
		else
		{
			// Ink and gap in turn; an odd count runs twice to come out even.
			std::vector<SkScalar> intervals;
			const size_t count = stroke.dashes.size() % 2 == 0 ? stroke.dashes.size() : stroke.dashes.size() * 2;
			float length = 0.0f;
			for (size_t i = 0; i < count; i++)
			{
				const float interval = stroke.dashes[i % stroke.dashes.size()] * stroke.width;
				intervals.push_back(interval);
				length += interval;
			}
			// Skia has one cap for the ends of the line and of the dashes: the dashes' round one.
			paint.setStrokeCap(stroke.flatCaps ? SkPaint::kButt_Cap : SkPaint::kRound_Cap);
			paint.setStrokeJoin(SkPaint::kMiter_Join);
			paint.setStrokeMiter(10.0f);
			if (length > 0.0f)
			{
				paint.setPathEffect(SkDashPathEffect::Make(SkSpan<const SkScalar>(intervals.data(), intervals.size()), 0.0f));
			}
		}

		// The width and the dashes are in device pixels whatever the transform, so the path is
		// taken to device pixels and stroked there.
		canvas.drawPath(path.makeTransform(matrix), paint);
	}

	void SkiaRenderer::Fill_(const SkPath& path, const Fill& fill)
	{
		SkPaint paint;
		paint.setAntiAlias(fill.smooth);   // off for areas: two side by side must not show a seam
		paint.setStyle(SkPaint::kFill_Style);
		if (fill.kind == Fill::Kind::Color)
		{
			paint.setColor4f(ToSk(fill.color), nullptr);
		}
		else
		{
			sk_sp<SkShader> pattern = Pattern(fill);
			if (!pattern)
			{
				omitted.patternFills++;
				return;
			}
			paint.setShader(std::move(pattern));
		}
		// The path goes to device pixels and the canvas stays as it is, so a pattern is laid in
		// device pixels from the corner of the surface whatever the transform.
		canvas.drawPath(path.makeTransform(matrix), paint);
	}

	sk_sp<SkShader> SkiaRenderer::Pattern(const Fill& fill)
	{
		if (!resources.areaFills || !resources.symbols || !resources.color)
		{
			return nullptr;
		}
		std::string key;
		if (fill.kind == Fill::Kind::AreaFill)
		{
			key = "a " + fill.reference + " " + fill.fallback;
		}
		else
		{
			key = "s " + fill.symbol;
			for (const float number : { fill.v1.x, fill.v1.y, fill.v2.x, fill.v2.y, fill.offset.x, fill.offset.y })
			{
				key += " " + std::to_string(number);
			}
		}
		const auto made = patterns.find(key);
		if (made != patterns.end())
		{
			return made->second;
		}

		// The tile is recorded as a picture (the shared code draws its symbols on a renderer of
		// this backend) and repeated; what a symbol at a corner has outside the tile is cut.
		sk_sp<SkShader> shader;
		const PatternTile tile = TileOf(fill, *resources.areaFills);
		if (!tile.Empty())
		{
			const SkRect bounds = SkRect::MakeWH(tile.width, tile.height);
			SkPictureRecorder recorder;
			SkiaRenderer onTile(*recorder.beginRecording(bounds), resources);
			if (DrawPatternTile(onTile, tile, *resources.symbols, resources.color))
			{
				shader = recorder.finishRecordingAsPicture()->makeShader(SkTileMode::kRepeat, SkTileMode::kRepeat,
					SkFilterMode::kNearest, nullptr, &bounds);
			}
		}
		patterns.emplace(std::move(key), shader);
		return shader;
	}

	void SkiaRenderer::StrokePath(const Path& path, const ColorF& color, const Stroke& stroke)
	{
		Stroke_(Build(path, true), color, stroke);
	}

	std::unique_ptr<RetainedPath> SkiaRenderer::Retain(const Path& path)
	{
		SkPath built = Build(path, true);
		if (built.isEmpty())
		{
			return nullptr;
		}
		auto retained = std::make_unique<RetainedSkPath>();
		retained->path = std::move(built);
		return retained;
	}

	void SkiaRenderer::StrokePath(const RetainedPath& path, const ColorF& color, const Stroke& stroke)
	{
		if (auto retained = dynamic_cast<const RetainedSkPath*>(&path))   // a path of another backend is not drawn
		{
			Stroke_(retained->path, color, stroke);
		}
	}

	void SkiaRenderer::FillPath(const Path& path, const Fill& fill)
	{
		Fill_(Build(path, false), fill);
	}

	void SkiaRenderer::FillPath(const RetainedPath& path, const Fill& fill)
	{
		if (auto retained = dynamic_cast<const RetainedSkPath*>(&path))
		{
			Fill_(retained->path, fill);
		}
	}

	std::unique_ptr<RetainedImage> SkiaRenderer::RetainImage(const std::uint8_t* pixels, int width, int height)
	{
		if (!pixels || width <= 0 || height <= 0)
		{
			return nullptr;
		}
		const SkImageInfo info = SkImageInfo::Make(width, height, kBGRA_8888_SkColorType, kPremul_SkAlphaType);
		auto retained = std::make_unique<RetainedSkImage>();
		retained->image = SkImages::RasterFromPixmapCopy(SkPixmap(info, pixels, static_cast<size_t>(width) * 4));
		if (!retained->image)
		{
			return nullptr;
		}
		return retained;
	}

	void SkiaRenderer::DrawImage(const RetainedImage& image, const ImageRect& source, const ImageRect& target)
	{
		auto retained = dynamic_cast<const RetainedSkImage*>(&image);
		if (!retained)
		{
			return;
		}
		// Under the painter's transform, each pixel of the image one colour.
		SkPaint paint;
		paint.setAntiAlias(false);
		canvas.save();
		canvas.concat(matrix);
		canvas.drawImageRect(retained->image, SkRect::MakeLTRB(source.left, source.top, source.right, source.bottom),
			SkRect::MakeLTRB(target.left, target.top, target.right, target.bottom), SkSamplingOptions(SkFilterMode::kNearest),
			&paint, SkCanvas::kFast_SrcRectConstraint);
		canvas.restore();
	}

	void SkiaRenderer::PushClip(const Path& path)
	{
		// The canvas keeps its matrix; the path goes to device pixels as every path does.
		canvas.save();
		canvas.clipPath(Build(path, false).makeTransform(matrix), true);
	}

	void SkiaRenderer::PopClip()
	{
		canvas.restore();
	}

	void SkiaRenderer::StrokeLineStyle(const std::string& reference, const std::vector<PointF>& points,
		const ColorF* shortStretches)
	{
		// The shared code lays the style along the line as paths and symbols, on this renderer,
		// under transforms of its own; the one in force comes back afterwards.
		const SkMatrix before = matrix;
		if (!resources.lineStyles || !resources.symbols || !resources.color
			|| !DrawLineStyle(*this, *resources.lineStyles, reference, points, shortStretches, resources.color))
		{
			omitted.lineStyles++;
		}
		matrix = before;
	}

	void SkiaRenderer::DrawSymbol(const std::string& reference, PointF at, float rotationDegrees, float scale)
	{
		const SymbolPicture* picture = resources.symbols && resources.color ? SymbolToDraw(*resources.symbols, reference) : nullptr;
		if (!picture)
		{
			omitted.symbols++;
			return;
		}

		// The symbol as an image drawn once, when there is somewhere to keep it.
		if (resources.symbolImages && DrawSymbolImage(*picture, at, rotationDegrees, scale))
		{
			return;
		}

		// The shared code draws the symbol as paths, on this renderer, under a transform of its
		// own; the one in force comes back afterwards.
		const SkMatrix before = matrix;
		DrawSymbolPicture(*this, *picture, at, rotationDegrees, scale, resources.color);
		matrix = before;
	}

	bool SkiaRenderer::DrawSymbolImage(const SymbolPicture& picture, PointF at, float rotationDegrees, float scale)
	{
		// An image is laid down pixel on pixel: on a canvas that is scaled or turned the symbol
		// is drawn from its shapes instead.
		const SkMatrix device = canvas.getTotalMatrix();
		if (!device.isTranslate())
		{
			return false;
		}

		const SkiaSymbolImages::Image* kept = resources.symbolImages->Find(&picture, rotationDegrees, scale);
		if (!kept)
		{
			// Drawn about the origin by the shared code, on a renderer of this backend that records:
			// the recording knows how far the marks of the symbol reach.
			SkRTreeFactory bounded;
			SkPictureRecorder recorder;
			const float far = 1.0e6f;
			{
				SkiaResources shapes = resources;
				shapes.symbolImages = nullptr;
				SkiaRenderer recording(*recorder.beginRecording(SkRect::MakeLTRB(-far, -far, far, far), &bounded), shapes);
				DrawSymbolPicture(recording, picture, { 0.0f, 0.0f }, SkiaSymbolImages::RoundedTurn(rotationDegrees), scale,
					resources.color);
			}
			const sk_sp<SkPicture> recorded = recorder.finishRecordingAsPicture();

			// A pixel more on every side, for the soft edge of the strokes.
			SkIRect bounds = recorded->cullRect().roundOut();
			bounds.outset(1, 1);
			if (recorded->cullRect().isEmpty())
			{
				kept = &resources.symbolImages->Keep(&picture, rotationDegrees, scale, {});
			}
			else if (bounds.width() > symbolImageMaxSide || bounds.height() > symbolImageMaxSide)
			{
				return false;
			}
			else
			{
				// Texts at full contrast, as on the surface of the view.
				const SkSurfaceProps props(0, kUnknown_SkPixelGeometry, SkSurfaceProps::kMaxContrastInclusive, 2.2f);
				const sk_sp<SkSurface> surface = SkSurfaces::Raster(
					SkImageInfo::MakeN32Premul(bounds.width(), bounds.height(), nullptr), &props);
				if (!surface)
				{
					return false;
				}
				SkCanvas* onImage = surface->getCanvas();
				onImage->clear(SK_ColorTRANSPARENT);
				onImage->translate(static_cast<float>(-bounds.left()), static_cast<float>(-bounds.top()));
				onImage->drawPicture(recorded);

				SkiaSymbolImages::Image image;
				image.image = surface->makeImageSnapshot();
				image.left = bounds.left();
				image.top = bounds.top();
				kept = &resources.symbolImages->Keep(&picture, rotationDegrees, scale, std::move(image));
			}
		}

		if (kept->image)
		{
			// The pivot goes to the nearest whole pixel of the surface, so the image is copied and
			// not sampled between pixels.
			const float x = std::round(at.x + device.getTranslateX()) - device.getTranslateX() + static_cast<float>(kept->left);
			const float y = std::round(at.y + device.getTranslateY()) - device.getTranslateY() + static_cast<float>(kept->top);
			canvas.drawImage(kept->image, x, y);
		}
		return true;
	}

	SizeF SkiaRenderer::MeasureText(const std::string& text, const TextFont& font)
	{
		if (!resources.fonts)
		{
			return {};
		}
		return LayOut(resources.fonts->Get(), text, font).size;
	}

	void SkiaRenderer::FillText(const std::string& text, PointF topLeft, const TextFont& font, const ColorF& color)
	{
		if (!resources.fonts || resources.fonts->Size() == 0)
		{
			omitted.texts++;
			return;
		}
		const TextLine line = LayOut(resources.fonts->Get(), text, font);

		SkPaint paint;
		paint.setAntiAlias(true);
		paint.setColor4f(ToSk(color), nullptr);

		canvas.save();
		canvas.concat(matrix);
		float x = topLeft.x;
		for (const TextRun& run : line.runs)
		{
			canvas.drawSimpleText(run.text.data(), run.text.size(), SkTextEncoding::kUTF8, x, topLeft.y + line.ascent,
				run.font, paint);
			x += run.advance;
		}
		canvas.restore();
	}
}
