// The S-100 side of drawing (DisplayListPainter) against a renderer that only records what it
// is asked to draw.
#include <gtest/gtest.h>

#include <cmath>
#include <filesystem>
#include <format>
#include <fstream>
#include <string>

#include "TestPaths.h"
#include "DisplayListJson.h"
#include "ColorPalette.h"
#include "SymbolPicture.h"
#include "PatternTile.h"
#include "LineStylePicture.h"
#include "DisplayListPainter.h"
#include "DrawingOrder.h"
#include "DrawingSet.h"
#include "FeatureCatalogue.h"
#include "IRenderer.h"
#include "PCOutputSchemaManager.h"
#include "S100Layer.h"
#include "S101Cell.h"
#include "SENC_DisplayList.h"
#include "SENC_Instruction.h"
#include "../GeoMetryLibrary/Scaler.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"

namespace
{
	// To a thousandth, as the consumer example writes it: the last bits of a coordinate differ
	// between platforms.
	float Q(float value)
	{
		value = std::round(value * 1000.0f) / 1000.0f;
		return value == 0 ? 0.0f : value;
	}

	// Writes every call down as a line of text.
	class RecordingRenderer : public s100::render::IRenderer
	{
	public:
		std::string text;
		int strokes = 0;
		int arcs = 0;
		int dashed = 0;

		// Every recorder is one backend, unless a test makes one another.
		static constexpr char recorder = 0;
		const void* backend = &recorder;

		const void* Backend() const override
		{
			return backend;
		}

		void SetTransform(const s100::render::Transform& transform) override
		{
			text += "transform";
			for (const auto& step : transform)
			{
				switch (step.kind)
				{
				case s100::render::TransformStep::Kind::Rotate:
					text += std::format(" rotate {} about {} {}", Q(step.degrees), Q(step.x), Q(step.y));
					break;
				case s100::render::TransformStep::Kind::Scale:
					text += std::format(" scale {:.6g}", step.factor);
					break;
				default:
					text += std::format(" translate {} {}", Q(step.x), Q(step.y));
					break;
				}
			}
			text += "\n";
		}

		void StrokePath(const s100::render::Path& path, const s100::render::ColorF& color,
			const s100::render::Stroke& stroke) override
		{
			strokes++;
			StrokeText(color, stroke);
			PathText(path);
			text += "\n";
		}

		// A kept path is written down once and is a number from then on.
		struct Kept : s100::render::RetainedPath
		{
			int number = 0;
		};

		std::unique_ptr<s100::render::RetainedPath> Retain(const s100::render::Path& path) override
		{
			auto kept = std::make_unique<Kept>();
			kept->number = ++retained;
			text += std::format("path {}", kept->number);
			PathText(path);
			text += "\n";
			return kept;
		}

		void StrokePath(const s100::render::RetainedPath& path, const s100::render::ColorF& color,
			const s100::render::Stroke& stroke) override
		{
			retainedStrokes++;
			StrokeText(color, stroke);
			text += std::format(" | path {}\n", static_cast<const Kept&>(path).number);
		}

		void FillPath(const s100::render::Path& path, const s100::render::Fill& fill) override
		{
			fills++;
			FillText(fill);
			PathText(path);
			text += "\n";
		}

		void FillPath(const s100::render::RetainedPath& path, const s100::render::Fill& fill) override
		{
			fills++;
			FillText(fill);
			text += std::format(" | path {}\n", static_cast<const Kept&>(path).number);
		}

		void StrokeLineStyle(const std::string& reference, const std::vector<s100::render::PointF>& points,
			const s100::render::ColorF* shortStretches) override
		{
			lineStyles++;
			text += "line style " + reference;
			if (shortStretches)
			{
				text += std::format(" color {} {} {} {}", Q(shortStretches->r), Q(shortStretches->g), Q(shortStretches->b), Q(shortStretches->a));
			}
			for (const auto& point : points)
			{
				text += std::format(" {} {}", Q(point.x), Q(point.y));
			}
			text += "\n";
		}

		int retained = 0;
		int retainedStrokes = 0;
		int lineStyles = 0;
		int fills = 0;
		int patternFills = 0;

	private:
		void FillText(const s100::render::Fill& fill)
		{
			switch (fill.kind)
			{
			case s100::render::Fill::Kind::Color:
				text += std::format("fill color {} {} {} {}", Q(fill.color.r), Q(fill.color.g), Q(fill.color.b), Q(fill.color.a));
				break;
			case s100::render::Fill::Kind::AreaFill:
				patternFills++;
				text += std::format("fill area fill {} or {}", fill.reference, fill.fallback.empty() ? "nothing" : fill.fallback);
				break;
			case s100::render::Fill::Kind::SymbolTiles:
				patternFills++;
				text += std::format("fill symbol {} lattice {} {} {} {} offset {} {}", fill.symbol, Q(fill.v1.x), Q(fill.v1.y),
					Q(fill.v2.x), Q(fill.v2.y), Q(fill.offset.x), Q(fill.offset.y));
				break;
			}
		}

		void StrokeText(const s100::render::ColorF& color, const s100::render::Stroke& stroke)
		{
			dashed += stroke.dashes.empty() ? 0 : 1;
			text += std::format("stroke width {} color {} {} {} {} dashes", Q(stroke.width), Q(color.r), Q(color.g), Q(color.b), Q(color.a));
			for (const float dash : stroke.dashes)
			{
				text += std::format(" {}", Q(dash));
			}
		}

		void PathText(const s100::render::Path& path)
		{
			for (const auto& figure : path.figures)
			{
				text += std::format(" | from {} {}", Q(figure.start.x), Q(figure.start.y));
				for (const auto& to : figure.lines)
				{
					text += std::format(" line {} {}", Q(to.x), Q(to.y));
				}
				text += figure.closed ? " closed" : " open";
			}
			for (const auto& arc : path.arcs)
			{
				arcs++;
				text += std::format(" | arc radius {} from {} to {}", Q(arc.radius), Q(arc.startDegrees), Q(arc.endDegrees));
			}
		}

	public:

		void DrawSymbol(const std::string& reference, s100::render::PointF at, float rotationDegrees, float scale) override
		{
			symbols++;
			text += std::format("symbol {} at {} {} rotation {} scale {}\n", reference, Q(at.x), Q(at.y), Q(rotationDegrees), Q(scale));
		}

		// A font with no glyphs: every byte of a text is half the font size wide.
		s100::render::SizeF MeasureText(const std::string& measured, const s100::render::TextFont& font) override
		{
			measures++;
			return { static_cast<float>(measured.size()) * font.size * 0.5f, font.size * 1.25f };
		}

		void FillText(const std::string& filled, s100::render::PointF topLeft, const s100::render::TextFont& font,
			const s100::render::ColorF& color) override
		{
			texts++;
			text += std::format("text \"{}\" at {} {} size {} {} color {} {} {} {}\n", filled, Q(topLeft.x), Q(topLeft.y),
				Q(font.size), font.italic ? "italic" : "upright", Q(color.r), Q(color.g), Q(color.b), Q(color.a));
		}

		int symbols = 0;
		int texts = 0;
		int measures = 0;
	};
}

// A sample cell with lights: its areas, lines, symbols, texts and augmented rays and paths (sector limits and arcs) drawn on the
// recording renderer. No Direct2D and no window: this is what runs on every platform.
TEST(DisplayListPainter, SampleCellIsPaintedAsRecorded)
{
	const auto root = testpaths::repoRoot();
	const auto cellPath = root / "SampleData" / "10100AA_X01NE.000";
	ASSERT_TRUE(std::filesystem::exists(cellPath)) << cellPath;

	FeatureCatalogue fc;
	ASSERT_TRUE(fc.Read((root / "ProgramData" / "FC" / "S-101_FC.xml").string()));
	PortrayalCatalogue pc;
	ASSERT_TRUE(pc.Open((root / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml").string()));

	S100Layer layer(&fc, &pc);
	auto* cell = new S101Cell(&fc);   // the layer owns its dataset
	layer.m_spatialObject = cell;
	cell->SetLayer(&layer);
	ASSERT_TRUE(cell->Open(cellPath.wstring()));
	layer.BuildPortrayalCatalogue();
	ASSERT_TRUE(cell->pcManager && cell->pcManager->displayListSENC);

	Scaler scaler;
	scaler.SetScreen(0, 0, 1600, 1000);
	scaler.SetMap(layer.m_mbr);

	RecordingRenderer renderer;
	int lookups = 0;
	s100::render::DisplayListPainter painter(renderer, scaler, [&lookups](const std::wstring& token) {
		// A colour made of the token, so the text shows which token was asked for.
		lookups++;
		return s100::render::ColorF{ static_cast<float>(token.size()), token.empty() ? 0.0f : static_cast<float>(token[0]), 0.0f, 1.0f };
	});

	int rays = 0;
	int paths = 0;
	for (auto& [key, instruction] : cell->pcManager->displayListSENC->displayInstructions)
	{
		rays += instruction->type == 7 ? 1 : 0;
		paths += instruction->type == 8 ? 1 : 0;
		instruction->Accept(painter);   // the kinds it has not taken over are left alone
	}

	EXPECT_EQ(rays, 10);
	EXPECT_EQ(paths, 10);
	EXPECT_EQ(renderer.strokes, 20);            // every ray is a stroke; here every path has one arc
	EXPECT_GT(renderer.symbols, 0);             // a point instruction is a symbol at each of its points
	EXPECT_GT(renderer.fills, 0);               // an area instruction is a fill of its surface
	EXPECT_GT(renderer.retained, 0);            // a curve is kept as a path once
	EXPECT_GE(renderer.retainedStrokes + renderer.fills, renderer.retained);   // and drawn for every instruction on it
	EXPECT_GT(renderer.lineStyles, 0);          // a line style of the catalogue goes to the backend by name
	EXPECT_GT(renderer.texts, 0);               // a text instruction is a text at each of its points
	EXPECT_GT(renderer.measures, 0);            // the painter asks the backend how large a text is
	EXPECT_GT(renderer.arcs, 0);
	EXPECT_GT(lookups, 0);

	const std::string hash = std::format("{:016x}", GISLibrary::JsonHash(renderer.text));
	if (hash != "e1711f127ddd47d8")
	{
		const auto dump = testpaths::tempDir() / "10100AA_X01NE.strokes.txt";
		std::ofstream(dump, std::ios::binary | std::ios::trunc) << renderer.text;
		ADD_FAILURE() << "recorded calls hash " << hash << ", strokes " << renderer.strokes << ", arcs " << renderer.arcs
			<< ", dashed " << renderer.dashed << ", symbols " << renderer.symbols << ", texts " << renderer.texts << ", measures " << renderer.measures
			<< ", kept paths " << renderer.retained << ", strokes of them " << renderer.retainedStrokes << ", line styles " << renderer.lineStyles
			<< ", fills " << renderer.fills << ", of them patterns " << renderer.patternFills << "; the text is in " << dump;
	}

	// Painted again, no curve is built again: its path is kept with it.
	const int kept = renderer.retained;
	const int strokesOfKept = renderer.retainedStrokes;
	const int fills = renderer.fills;
	for (auto& [key, instruction] : cell->pcManager->displayListSENC->displayInstructions)
	{
		instruction->Accept(painter);
	}
	EXPECT_EQ(renderer.retained, kept);
	EXPECT_EQ(renderer.retainedStrokes, 2 * strokesOfKept);
	EXPECT_EQ(renderer.fills, 2 * fills);

	// Another backend does not draw with what this one made: it gets paths and sizes of its own.
	RecordingRenderer other;
	other.backend = &other;
	s100::render::DisplayListPainter otherPainter(other, scaler, [](const std::wstring&) { return s100::render::ColorF{}; });
	for (auto& [key, instruction] : cell->pcManager->displayListSENC->displayInstructions)
	{
		instruction->Accept(otherPainter);
	}
	EXPECT_EQ(other.retained, kept);
	EXPECT_EQ(other.retainedStrokes, strokesOfKept);
	EXPECT_EQ(other.fills, fills);
	EXPECT_EQ(other.measures, renderer.measures);

	// And the first one, back, builds its own again.
	for (auto& [key, instruction] : cell->pcManager->displayListSENC->displayInstructions)
	{
		instruction->Accept(painter);
	}
	EXPECT_EQ(renderer.retained, 2 * kept);
}

namespace
{
	// Writes the stages of a priority into the text of the recorder, between the calls.
	class RecordingStages : public s100::render::DrawingStages
	{
	public:
		explicit RecordingStages(std::string& text) : text(text)
		{
		}

		void BeginAreasAndLines() override { text += "stage areas and lines\n"; }
		void BeforeArea(SENC_Instruction&) override { areas++; }
		void BeforeLine(SENC_Instruction&) override { lines++; }
		void EndAreasAndLines() override { text += "stage end of areas and lines\n"; }
		void BeginTexts(bool any) override { text += any ? "stage texts\n" : "stage no texts\n"; }
		void BeforeText(SENC_Instruction&) override { texts++; }
		void EndTexts() override { text += "stage end of texts\n"; }

		int areas = 0;
		int lines = 0;
		int texts = 0;

	private:
		std::string& text;
	};
}

// A frame of the sample cell as a display draws it: the instructions the view shows, sorted by
// drawing priority and kind, the shared curves suppressed, one priority after another.
TEST(DrawingOrder, SampleCellFrameIsPaintedInOrder)
{
	const auto root = testpaths::repoRoot();
	const auto cellPath = root / "SampleData" / "10100AA_X01NE.000";
	ASSERT_TRUE(std::filesystem::exists(cellPath)) << cellPath;

	FeatureCatalogue fc;
	ASSERT_TRUE(fc.Read((root / "ProgramData" / "FC" / "S-101_FC.xml").string()));
	PortrayalCatalogue pc;
	ASSERT_TRUE(pc.Open((root / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml").string()));

	S100Layer layer(&fc, &pc);
	auto* cell = new S101Cell(&fc);   // the layer owns its dataset
	layer.m_spatialObject = cell;
	cell->SetLayer(&layer);
	ASSERT_TRUE(cell->Open(cellPath.wstring()));
	layer.BuildPortrayalCatalogue();
	ASSERT_TRUE(cell->pcManager && cell->pcManager->displayListSENC);
	auto& displayList = *cell->pcManager->displayListSENC;

	Scaler scaler;
	scaler.SetScreen(0, 0, 1600, 1000);
	scaler.SetMap(layer.m_mbr);

	// Every feature type shown: the set has an instruction for all the display list gives.
	DrawingSet set;
	std::set<int> priorities;
	s100::render::CollectInstructions(displayList, scaler.GetCurrentScale(), scaler, nullptr, set, priorities);
	ASSERT_FALSE(priorities.empty());
	size_t collected = 0;
	for (const int priority : priorities)
	{
		ASSERT_GE(priority, 0);
		ASSERT_LT(priority, 100);
		collected += set.GetAreaList(priority)->size() + set.GetLineList(priority)->size() + set.GetPointList(priority)->size()
			+ set.GetTextList(priority)->size() + set.GetAugmentedRayList(priority)->size() + set.GetAugmentedPathList(priority)->size();
	}
	EXPECT_GT(collected, 0u);
	EXPECT_LE(collected, displayList.displayInstructions.size());

	// A feature type turned off is not collected.
	DrawingSet withoutLights;
	std::set<int> prioritiesWithoutLights;
	s100::render::CollectInstructions(displayList, scaler.GetCurrentScale(), scaler,
		[](const std::wstring& code) { return code != L"LightSectored"; }, withoutLights, prioritiesWithoutLights);
	size_t collectedWithoutLights = 0;
	for (const int priority : prioritiesWithoutLights)
	{
		collectedWithoutLights += withoutLights.GetAreaList(priority)->size() + withoutLights.GetLineList(priority)->size()
			+ withoutLights.GetPointList(priority)->size() + withoutLights.GetTextList(priority)->size()
			+ withoutLights.GetAugmentedRayList(priority)->size() + withoutLights.GetAugmentedPathList(priority)->size();
	}
	EXPECT_LT(collectedWithoutLights, collected);

	s100::render::SuppressSharedCurves(priorities, set);

	RecordingRenderer renderer;
	s100::render::DisplayListPainter painter(renderer, scaler, [](const std::wstring& token) {
		return s100::render::ColorF{ static_cast<float>(token.size()), token.empty() ? 0.0f : static_cast<float>(token[0]), 0.0f, 1.0f };
	});
	RecordingStages stages(renderer.text);
	for (const int priority : priorities)
	{
		renderer.text += std::format("priority {}\n", priority);
		s100::render::PaintPriority(priority, set, painter, true, &stages);
	}
	EXPECT_GT(stages.areas, 0);
	EXPECT_GT(stages.lines, 0);
	EXPECT_GT(stages.texts, 0);
	EXPECT_GT(renderer.fills, 0);
	EXPECT_GT(renderer.texts, 0);

	const std::string hash = std::format("{:016x}", GISLibrary::JsonHash(renderer.text));
	if (hash != "09caae70ea586ed1")
	{
		const auto dump = testpaths::tempDir() / "10100AA_X01NE.frame.txt";
		std::ofstream(dump, std::ios::binary | std::ios::trunc) << renderer.text;
		ADD_FAILURE() << "recorded frame hash " << hash << ", priorities " << priorities.size() << ", instructions " << collected
			<< ", symbols " << renderer.symbols << ", texts " << renderer.texts << ", fills " << renderer.fills
			<< ", strokes " << renderer.strokes << ", strokes of kept paths " << renderer.retainedStrokes
			<< ", line styles " << renderer.lineStyles << "; the text is in " << dump;
	}

	// With texts off, none is drawn and the stages of texts are not entered.
	RecordingRenderer silent;
	s100::render::DisplayListPainter silentPainter(silent, scaler, [](const std::wstring&) { return s100::render::ColorF{}; });
	RecordingStages silentStages(silent.text);
	for (const int priority : priorities)
	{
		s100::render::PaintPriority(priority, set, silentPainter, false, &silentStages);
	}
	EXPECT_EQ(silent.texts, 0);
	EXPECT_EQ(silentStages.texts, 0);
	EXPECT_EQ(silent.fills, renderer.fills);
}

// The colours of the tokens come from the colour profile of the catalogue, read with no backend.
TEST(ColorPalette, ReadsThePalettesOfTheS101Catalogue)
{
	PortrayalCatalogue pc;
	ASSERT_TRUE(pc.Open((testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml").string()));

	s100::render::ColorPalette day;
	ASSERT_TRUE(day.Read(pc));
	EXPECT_GT(day.Size(), 50u);

	// Deep water by day: sRGB 201, 237, 255 in the colour profile.
	const auto deep = day.Color(L"DEPDW");
	EXPECT_FLOAT_EQ(deep.r, 201 / 255.0f);
	EXPECT_FLOAT_EQ(deep.g, 237 / 255.0f);
	EXPECT_FLOAT_EQ(deep.b, 1.0f);
	EXPECT_FLOAT_EQ(deep.a, 1.0f);

	// A token the palette does not have is black.
	const auto unknown = day.Color(L"NO_SUCH_TOKEN");
	EXPECT_FLOAT_EQ(unknown.r + unknown.g + unknown.b, 0.0f);
	EXPECT_FLOAT_EQ(unknown.a, 1.0f);

	s100::render::ColorPalette night;
	ASSERT_TRUE(night.Read(pc, "Night"));
	EXPECT_EQ(night.Size(), day.Size());
	EXPECT_LT(night.Color(L"DEPDW").r, 0.5f);

	s100::render::ColorPalette none;
	EXPECT_FALSE(none.Read(pc, "Noon"));
	EXPECT_EQ(none.Size(), 0u);
}

namespace
{
	std::filesystem::path WriteSymbol(const char* name, const char* body)
	{
		const auto path = testpaths::tempDir() / name;
		std::ofstream(path, std::ios::binary | std::ios::trunc)
			<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<svg xmlns=\"http://www.w3.org/2000/svg\">\n" << body << "</svg>\n";
		return path;
	}
}

// A symbol file is read into shapes: what is drawn, how it is coloured, how wide its lines are.
TEST(SymbolPicture, ReadsTheShapesOfASymbolFile)
{
	const auto path = WriteSymbol("TESTSYM1.svg",
		"<title>a test symbol</title>\n"
		"<rect class=\"symbolBox layout\" fill=\"none\" x=\"-9\" y=\"-9\" height=\"18\" width=\"18\"/>\n"
		"<rect class=\"svgBox layout\" fill=\"none\" x=\"-2\" y=\"-3\" height=\"6\" width=\"4\"/>\n"
		"<path d=\" M -1,-2 L 1,-2 L 1,2\" class=\"sl f0 sCHBLK\" stroke-width=\"0.32\" />\n"
		"<path d=\"M 0,0 L 2,0 L 2,2 Z M 5,5 L 6,5 6,6\" class=\"fCHMGD\" style=\"fill-opacity:0.5\" />\n"
		"<path d=\"m 1,1 h 2 v 3 H 0 V 0 l 1,1 z\" class=\"sl f0 sCHRED\" style=\"stroke-width:0.64;\" />\n"
		"<path d=\"M 0,0 L 1,1 C 1,2 3,4 5,6 L 9,9\" class=\"sl f0 sCHBLK\" stroke-width=\"0.32\" />\n"
		"<rect class=\"sl f0 sCHGRD\" x=\"1\" y=\"2\" width=\"3\" height=\"4\" stroke-width=\"0.3\"/>\n"
		"<circle class=\"fCHYLW\" cx=\"1\" cy=\"-1\" r=\"0.5\"/>\n"
		"<circle class=\"pivotPoint layout\" fill=\"none\" cx=\"0\" cy=\"0\" r=\"1\"/>\n"
		"<g><path d=\"M 0,0 L 1,1\" class=\"sl f0 sCHBLK\" stroke-width=\"0.32\"/></g>\n");

	s100::render::SymbolPicture picture;
	ASSERT_TRUE(s100::render::ReadSymbolPicture(path, picture));
	EXPECT_EQ(picture.name, "TESTSYM1");
	EXPECT_FLOAT_EQ(picture.x, -2.0f);
	EXPECT_FLOAT_EQ(picture.y, -3.0f);
	EXPECT_FLOAT_EQ(picture.width, 4.0f);
	EXPECT_FLOAT_EQ(picture.height, 6.0f);
	EXPECT_EQ(picture.unsupported, 1);   // the path with a curve
	ASSERT_EQ(picture.shapes.size(), 6u);   // the layout boxes, the pivot and the group are no shapes

	// An open line, stroked.
	const auto& line = picture.shapes[0];
	EXPECT_FALSE(line.fill);
	EXPECT_EQ(line.colorToken, "CHBLK");
	EXPECT_FLOAT_EQ(line.strokeWidth, 0.32f);
	EXPECT_FLOAT_EQ(line.opacity, 1.0f);
	ASSERT_EQ(line.figures.size(), 1u);
	EXPECT_FALSE(line.figures[0].closed);
	EXPECT_FLOAT_EQ(line.figures[0].start.x, -1.0f);
	ASSERT_EQ(line.figures[0].lines.size(), 2u);
	EXPECT_FLOAT_EQ(line.figures[0].lines[1].y, 2.0f);

	// Two figures in one path: a closed one, and an open one whose last pair has no letter.
	const auto& filled = picture.shapes[1];
	EXPECT_TRUE(filled.fill);
	EXPECT_EQ(filled.colorToken, "CHMGD");
	EXPECT_FLOAT_EQ(filled.opacity, 0.5f);
	ASSERT_EQ(filled.figures.size(), 2u);
	EXPECT_TRUE(filled.figures[0].closed);
	EXPECT_EQ(filled.figures[0].lines.size(), 2u);
	EXPECT_FALSE(filled.figures[1].closed);
	EXPECT_FLOAT_EQ(filled.figures[1].start.x, 5.0f);
	ASSERT_EQ(filled.figures[1].lines.size(), 2u);
	EXPECT_FLOAT_EQ(filled.figures[1].lines[1].x, 6.0f);
	EXPECT_FLOAT_EQ(filled.figures[1].lines[1].y, 6.0f);

	// Relative commands, horizontal and vertical lines.
	const auto& relative = picture.shapes[2];
	EXPECT_EQ(relative.colorToken, "CHRED");
	EXPECT_FLOAT_EQ(relative.strokeWidth, 0.64f);
	ASSERT_EQ(relative.figures.size(), 1u);
	EXPECT_TRUE(relative.figures[0].closed);
	EXPECT_FLOAT_EQ(relative.figures[0].start.x, 1.0f);
	ASSERT_EQ(relative.figures[0].lines.size(), 5u);
	EXPECT_FLOAT_EQ(relative.figures[0].lines[0].x, 3.0f);   // h 2
	EXPECT_FLOAT_EQ(relative.figures[0].lines[1].y, 4.0f);   // v 3
	EXPECT_FLOAT_EQ(relative.figures[0].lines[2].x, 0.0f);   // H 0
	EXPECT_FLOAT_EQ(relative.figures[0].lines[3].y, 0.0f);   // V 0
	EXPECT_FLOAT_EQ(relative.figures[0].lines[4].x, 1.0f);   // l 1,1
	EXPECT_FLOAT_EQ(relative.figures[0].lines[4].y, 1.0f);

	// A path with a curve is kept up to the curve.
	ASSERT_EQ(picture.shapes[3].figures.size(), 1u);
	EXPECT_EQ(picture.shapes[3].figures[0].lines.size(), 1u);

	// A rectangle that is not layout is a closed figure.
	const auto& box = picture.shapes[4];
	EXPECT_EQ(box.colorToken, "CHGRD");
	ASSERT_EQ(box.figures.size(), 1u);
	EXPECT_TRUE(box.figures[0].closed);
	EXPECT_FLOAT_EQ(box.figures[0].lines[1].x, 4.0f);
	EXPECT_FLOAT_EQ(box.figures[0].lines[1].y, 6.0f);

	const auto& circle = picture.shapes[5];
	EXPECT_TRUE(circle.fill);
	EXPECT_EQ(circle.colorToken, "CHYLW");
	EXPECT_FLOAT_EQ(circle.radius, 0.5f);
	EXPECT_FLOAT_EQ(circle.center.y, -1.0f);

	s100::render::SymbolPicture none;
	EXPECT_FALSE(s100::render::ReadSymbolPicture(testpaths::tempDir() / "NO_SUCH_SYMBOL.svg", none));
}

// A symbol is drawn as paths: scaled, turned and moved to its place, fills antialiased, strokes
// as wide on the screen as their millimetres say whatever the scale.
TEST(SymbolPicture, IsDrawnAsPaths)
{
	const auto path = WriteSymbol("TESTSYM2.svg",
		"<path d=\"M 0,0 L 2,0 L 2,2 Z\" class=\"fCHMGD\" style=\"fill-opacity:0.5\" />\n"
		"<path d=\"M -1,-2 L 1,-2\" class=\"sl f0 sCHBLK\" stroke-width=\"0.32\" />\n"
		"<path d=\"M -1,-2 L 1,-2\" class=\"sl f0 sCHBLK\" />\n"
		"<circle class=\"sl f0 sCHYLW\" cx=\"0\" cy=\"0\" r=\"1\" stroke-width=\"0.5\"/>\n");
	s100::render::SymbolPicture picture;
	ASSERT_TRUE(s100::render::ReadSymbolPicture(path, picture));

	RecordingRenderer renderer;
	s100::render::DrawSymbolPicture(renderer, picture, { 100.0f, 200.0f }, 90.0f, 5.0f, [](const std::wstring& token) {
		return s100::render::ColorF{ static_cast<float>(token.size()), 0.0f, 0.0f, 1.0f };
	});

	EXPECT_EQ(renderer.fills, 1);
	EXPECT_EQ(renderer.strokes, 2);   // the line with no width is not stroked
	EXPECT_NE(renderer.text.find("transform scale 5 rotate 90 about 0 0 translate 100 200\n"), std::string::npos) << renderer.text;
	// The fill takes the opacity of the shape; the stroke is 0.32 mm at 96 pixels to an inch.
	EXPECT_NE(renderer.text.find("fill color 5 0 0 0.5 | from 0 0 line 2 0 line 2 2 closed"), std::string::npos) << renderer.text;
	EXPECT_NE(renderer.text.find("stroke width 1.209 color 5 0 0 1 dashes | from -1 -2 line 1 -2 open"), std::string::npos) << renderer.text;
	// The circle: a closed figure of chords, 4 pi r of them at 5 pixels to the millimetre.
	EXPECT_NE(renderer.text.find("stroke width 1.89 color 5 0 0 1 dashes | from 1 0 line"), std::string::npos) << renderer.text;
	EXPECT_EQ(std::count(renderer.text.begin(), renderer.text.end(), '\n'), 4);
}

// Every symbol of the S-101 catalogue is read whole, and a name the catalogue does not have is
// drawn as its question mark.
TEST(SymbolPicture, ReadsTheSymbolsOfTheS101Catalogue)
{
	PortrayalCatalogue pc;
	ASSERT_TRUE(pc.Open((testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml").string()));

	s100::render::SymbolLibrary symbols(pc);
	EXPECT_GT(symbols.Size(), 500u);

	size_t read = 0;
	size_t shapes = 0;
	int unsupported = 0;
	for (auto file : *pc.GetSymbols()->GetSymbolsVector())
	{
		const std::string name = std::filesystem::path(file->GetFileNameW()).stem().string();
		const auto picture = symbols.Find(name);
		ASSERT_NE(picture, nullptr) << name;
		EXPECT_EQ(picture, symbols.Find(name));   // read once
		read++;
		shapes += picture->shapes.size();
		unsupported += picture->unsupported;
	}
	EXPECT_EQ(read, symbols.Size());
	EXPECT_GT(shapes, read);
	EXPECT_EQ(unsupported, 0);

	EXPECT_EQ(symbols.Find("NO_SUCH_SYMBOL"), nullptr);

	const auto color = [](const std::wstring&) { return s100::render::ColorF{}; };
	RecordingRenderer unknown;
	EXPECT_TRUE(s100::render::DrawSymbol(unknown, symbols, "NO_SUCH_SYMBOL", { 0.0f, 0.0f }, 0.0f, 5.0f, color));
	RecordingRenderer questionMark;
	EXPECT_TRUE(s100::render::DrawSymbol(questionMark, symbols, "QUESMRK1", { 0.0f, 0.0f }, 0.0f, 5.0f, color));
	EXPECT_FALSE(unknown.text.empty());
	EXPECT_EQ(unknown.text, questionMark.text);
}

// The tile of a pattern: a rectangle of whole pixels with the symbol at its corners, and in its
// middle too when every other row of the lattice is shifted.
TEST(PatternTile, IsMadeFromTheLattice)
{
	const auto square = s100::render::MakePatternTile("SYMBOL", { 22.5f, 0.0f }, { 0.0f, 43.13f }, { 0.0f, 0.0f }, 5.0f);
	EXPECT_FALSE(square.Empty());
	EXPECT_EQ(square.symbol, "SYMBOL");
	EXPECT_FLOAT_EQ(square.width, 112.0f);    // 112.5 cut to whole pixels
	EXPECT_FLOAT_EQ(square.height, 215.0f);   // 215.65
	ASSERT_EQ(square.at.size(), 4u);
	EXPECT_FLOAT_EQ(square.at[2].x, 112.5f);  // the places are not cut
	EXPECT_FLOAT_EQ(square.at[2].y, 215.65f);

	const auto staggered = s100::render::MakePatternTile("SYMBOL", { 10.0f, 0.0f }, { 5.0f, 4.0f }, { 1.0f, 2.0f }, 5.0f);
	EXPECT_FLOAT_EQ(staggered.width, 50.0f);
	EXPECT_FLOAT_EQ(staggered.height, 40.0f);   // two rows
	ASSERT_EQ(staggered.at.size(), 5u);
	EXPECT_FLOAT_EQ(staggered.at[0].x, 5.0f);   // the offset moves every symbol
	EXPECT_FLOAT_EQ(staggered.at[0].y, 10.0f);
	EXPECT_FLOAT_EQ(staggered.at[2].x, 30.0f);  // the one in the middle: half a step along, one row down
	EXPECT_FLOAT_EQ(staggered.at[2].y, 30.0f);
	EXPECT_FLOAT_EQ(staggered.at[4].x, 55.0f);
	EXPECT_FLOAT_EQ(staggered.at[4].y, 50.0f);

	EXPECT_TRUE(s100::render::MakePatternTile("SYMBOL", { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, 5.0f).Empty());
}

// The area fills of the S-101 catalogue are read from their files, and a pattern fill is the
// tile of the one it names.
TEST(PatternTile, ReadsTheAreaFillsOfTheS101Catalogue)
{
	PortrayalCatalogue pc;
	ASSERT_TRUE(pc.Open((testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml").string()));

	s100::render::AreaFillLibrary areaFills(pc);
	EXPECT_EQ(areaFills.Size(), pc.GetAreaFills()->GetAreaFillFilesVecter()->size());
	EXPECT_GT(areaFills.Size(), 20u);

	const auto diamond = areaFills.Find("DIAMOND1");
	ASSERT_NE(diamond, nullptr);
	EXPECT_EQ(diamond->name, "DIAMOND1");
	EXPECT_EQ(diamond->symbol, "DIAMOND1P");
	EXPECT_FLOAT_EQ(diamond->v1.x, 22.5f);
	EXPECT_FLOAT_EQ(diamond->v2.y, 43.13f);
	EXPECT_EQ(areaFills.Find("diamond1"), diamond);   // whatever the case
	EXPECT_EQ(areaFills.Find("NO_SUCH_FILL"), nullptr);

	// Every area fill names a symbol the catalogue has.
	s100::render::SymbolLibrary symbols(pc);
	for (auto file : *pc.GetAreaFills()->GetAreaFillFilesVecter())
	{
		const std::string name = std::filesystem::path(file->GetFileNameW()).stem().string();
		const auto areaFill = areaFills.Find(name);
		ASSERT_NE(areaFill, nullptr) << name;
		EXPECT_NE(symbols.Find(areaFill->symbol), nullptr) << name << " " << areaFill->symbol;
	}

	s100::render::Fill byName;
	byName.kind = s100::render::Fill::Kind::AreaFill;
	byName.reference = "DIAMOND1";
	const auto tile = s100::render::TileOf(byName, areaFills);
	EXPECT_EQ(tile.symbol, "DIAMOND1P");
	EXPECT_FLOAT_EQ(tile.width, 112.0f);
	EXPECT_FLOAT_EQ(tile.height, 215.0f);

	// A name the catalogue does not have: the fallback, or nothing.
	s100::render::Fill unknown;
	unknown.kind = s100::render::Fill::Kind::AreaFill;
	unknown.reference = "NO_SUCH_FILL";
	EXPECT_TRUE(s100::render::TileOf(unknown, areaFills).Empty());
	unknown.fallback = "QUESMRK1";
	EXPECT_FALSE(s100::render::TileOf(unknown, areaFills).Empty());

	// A lattice of the rule's own.
	s100::render::Fill lattice;
	lattice.kind = s100::render::Fill::Kind::SymbolTiles;
	lattice.symbol = "DIAMOND1P";
	lattice.v1 = { 10.0f, 0.0f };
	lattice.v2 = { 0.0f, 8.0f };
	const auto own = s100::render::TileOf(lattice, areaFills);
	EXPECT_FLOAT_EQ(own.width, 50.0f);
	EXPECT_FLOAT_EQ(own.height, 40.0f);

	// The symbols of a tile are drawn at its places; with a symbol the catalogue lacks, nothing.
	const auto color = [](const std::wstring&) { return s100::render::ColorF{}; };
	RecordingRenderer renderer;
	EXPECT_TRUE(s100::render::DrawPatternTile(renderer, tile, symbols, color));
	EXPECT_NE(renderer.text.find("transform scale 5 rotate 0 about 0 0 translate 112.5 215.65\n"), std::string::npos) << renderer.text;
	lattice.symbol = "NO_SUCH_SYMBOL";
	RecordingRenderer none;
	EXPECT_FALSE(s100::render::DrawPatternTile(none, s100::render::TileOf(lattice, areaFills), symbols, color));
	EXPECT_TRUE(none.text.empty());
}

// A line style file is read into dashes and symbols, in pixels along the line.
TEST(LineStylePicture, ReadsTheLineStylesOfTheS101Catalogue)
{
	PortrayalCatalogue pc;
	ASSERT_TRUE(pc.Open((testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml").string()));

	s100::render::LineStyleLibrary lineStyles(pc);
	EXPECT_EQ(lineStyles.Size(), pc.GetTreeLineStyles()->GetLineStyleFilesVector()->size());
	EXPECT_GT(lineStyles.Size(), 50u);

	// 32.3 mm at 6.25 pixels to the millimetre is 201.875: the interval is cut to whole pixels,
	// the dashes and the places of the symbols are not.
	const auto anchorage = lineStyles.Find("ACHARE51");
	ASSERT_NE(anchorage, nullptr);
	EXPECT_FLOAT_EQ(anchorage->length, 201.0f);
	EXPECT_EQ(anchorage->colorToken, "CHMGD");
	EXPECT_FLOAT_EQ(anchorage->opacity, 1.0f);
	ASSERT_EQ(anchorage->dashes.size(), 3u);
	EXPECT_FLOAT_EQ(anchorage->dashes[1].start, 113.75f);
	EXPECT_FLOAT_EQ(anchorage->dashes[1].length, 37.5f);
	ASSERT_EQ(anchorage->symbols.size(), 4u);
	EXPECT_EQ(anchorage->symbols[1].reference, "EMACHAR1");
	EXPECT_FLOAT_EQ(anchorage->symbols[1].position, 81.875f);
	EXPECT_TRUE(anchorage->lines.empty());

	// A composite line style: lines side by side.
	const auto boundary = lineStyles.Find("SCLBDY51");
	ASSERT_NE(boundary, nullptr);
	ASSERT_EQ(boundary->lines.size(), 3u);
	EXPECT_FLOAT_EQ(boundary->lines[0].offset, -3.125f);
	EXPECT_FLOAT_EQ(boundary->lines[0].width, 3.0f);
	EXPECT_EQ(boundary->lines[0].colorToken, "CHGRF");

	// Every symbol a line style names is in the catalogue.
	s100::render::SymbolLibrary symbols(pc);
	for (auto file : *pc.GetTreeLineStyles()->GetLineStyleFilesVector())
	{
		const std::string name = std::filesystem::path(file->GetFileNameW()).stem().string();
		const auto lineStyle = lineStyles.Find(name);
		ASSERT_NE(lineStyle, nullptr) << name;
		EXPECT_TRUE(lineStyle->length >= 1.0f || !lineStyle->lines.empty()) << name;
		for (const auto& symbol : lineStyle->symbols)
		{
			EXPECT_NE(symbols.Find(symbol.reference), nullptr) << name << " " << symbol.reference;
		}
	}
	EXPECT_EQ(lineStyles.Find("NO_SUCH_STYLE"), nullptr);
}

// A line style is laid along each stretch of a line from its first point: dashes cut where the
// stretch ends, symbols whole where their place is still on it, a short stretch a thin dashed line.
TEST(LineStylePicture, IsLaidAlongALine)
{
	PortrayalCatalogue pc;
	ASSERT_TRUE(pc.Open((testpaths::repoRoot() / "ProgramData" / "PC" / "S101_Portrayal" / "portrayal_catalogue.xml").string()));
	s100::render::SymbolLibrary symbols(pc);
	const auto color = [](const std::wstring& token) {
		return s100::render::ColorF{ static_cast<float>(token.size()), 0.0f, 0.0f, 1.0f };
	};

	s100::render::LineStylePicture style;
	style.length = 100.0f;
	style.colorToken = "CHBLK";
	style.dashes = { { 10.0f, 30.0f }, { 60.0f, 30.0f } };
	style.symbols = { { "QUESMRK1", 50.0f, 0.0f } };

	// 250 pixels to the right: two intervals and a half. The second dash of the half is past the
	// end; the symbol of the half is at 250, the very end.
	RecordingRenderer renderer;
	s100::render::DrawLineStyle(renderer, style, { { 100.0f, 200.0f }, { 350.0f, 200.0f } }, nullptr, color);
	EXPECT_NE(renderer.text.find("transform rotate 0 about 100 200\n"), std::string::npos) << renderer.text;
	EXPECT_NE(renderer.text.find("stroke width 1 color 5 0 0 1 dashes"
		" | from 110 200 line 140 200 open | from 160 200 line 190 200 open"
		" | from 210 200 line 240 200 open | from 260 200 line 290 200 open"
		" | from 310 200 line 340 200 open\n"), std::string::npos) << renderer.text;
	EXPECT_NE(renderer.text.find("symbol QUESMRK1 at 150 200 rotation 0 scale 5\n"), std::string::npos) << renderer.text;
	EXPECT_NE(renderer.text.find("symbol QUESMRK1 at 250 200 rotation 0 scale 5\n"), std::string::npos) << renderer.text;
	EXPECT_NE(renderer.text.find("symbol QUESMRK1 at 350 200 rotation 0 scale 5\n"), std::string::npos) << renderer.text;

	// A stretch of exactly two intervals is drawn (the viewer left such a stretch out).
	RecordingRenderer exact;
	s100::render::DrawLineStyle(exact, style, { { 0.0f, 0.0f }, { 200.0f, 0.0f } }, nullptr, color);
	EXPECT_NE(exact.text.find(" | from 160 0 line 190 0 open\n"), std::string::npos) << exact.text;

	// Down the screen: the strip is turned, the dashes are still along x from the first point;
	// the symbol is placed on the surface and turned with the line. A dash is cut at the end.
	RecordingRenderer down;
	s100::render::DrawLineStyle(down, style, { { 10.0f, 10.0f }, { 10.0f, 80.0f } }, nullptr, color);
	EXPECT_NE(down.text.find("transform rotate 90.003 about 10 10\n"), std::string::npos) << down.text;
	EXPECT_NE(down.text.find(" | from 20 10 line 50 10 open | from 70 10 line 80 10 open\n"), std::string::npos) << down.text;
	EXPECT_NE(down.text.find("symbol QUESMRK1 at 10 60 rotation 90.003 scale 5\n"), std::string::npos) << down.text;

	// A short stretch with a colour for it: a thin dashed line, in device pixels.
	const s100::render::ColorF red{ 1.0f, 0.0f, 0.0f, 1.0f };
	RecordingRenderer shortLine;
	s100::render::DrawLineStyle(shortLine, style, { { 0.0f, 0.0f }, { 12.0f, 0.0f } }, &red, color);
	EXPECT_EQ(shortLine.text, "transform\nstroke width 1 color 1 0 0 1 dashes 2 2 | from 0 0 line 12 0 open\n");
	// With no colour for it, the strip.
	RecordingRenderer shortStrip;
	s100::render::DrawLineStyle(shortStrip, style, { { 0.0f, 0.0f }, { 12.0f, 0.0f } }, nullptr, color);
	EXPECT_NE(shortStrip.text.find(" | from 10 0 line 12 0 open\n"), std::string::npos) << shortStrip.text;

	// A composite line style: its lines along the whole stretch, each at its offset.
	s100::render::LineStylePicture composite;
	composite.lines = { { -3.0f, 2.0f, "CHGRF", 1.0f }, { 5.0f, 1.0f, "CHGRF", 0.5f } };
	RecordingRenderer sideBySide;
	s100::render::DrawLineStyle(sideBySide, composite, { { 0.0f, 0.0f }, { 50.0f, 0.0f } }, nullptr, color);
	EXPECT_NE(sideBySide.text.find("stroke width 2 color 5 0 0 1 dashes | from 0 -3 line 50 -3 open\n"), std::string::npos) << sideBySide.text;
	EXPECT_NE(sideBySide.text.find("stroke width 1 color 5 0 0 0.5 dashes | from 0 5 line 50 5 open\n"), std::string::npos) << sideBySide.text;

	// A name the catalogue does not have is drawn as its question mark.
	s100::render::LineStyleLibrary lineStyles(pc);
	RecordingRenderer unknown;
	EXPECT_TRUE(s100::render::DrawLineStyle(unknown, lineStyles, "NO_SUCH_STYLE", { { 0.0f, 0.0f }, { 300.0f, 0.0f } }, nullptr, color));
	RecordingRenderer questionMark;
	EXPECT_TRUE(s100::render::DrawLineStyle(questionMark, lineStyles, "QUESMRK1", { { 0.0f, 0.0f }, { 300.0f, 0.0f } }, nullptr, color));
	EXPECT_FALSE(unknown.text.empty());
	EXPECT_EQ(unknown.text, questionMark.text);
}
