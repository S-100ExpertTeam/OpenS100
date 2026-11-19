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
#include "DisplayListPainter.h"
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

	private:
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

// A sample cell with lights: its symbols, lines, texts and augmented rays and paths (sector limits and arcs) drawn on the
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
	EXPECT_GT(renderer.retained, 0);            // a curve is kept as a path once
	EXPECT_GE(renderer.retainedStrokes, renderer.retained);   // and stroked for every instruction on it
	EXPECT_GT(renderer.lineStyles, 0);          // a line style of the catalogue goes to the backend by name
	EXPECT_GT(renderer.texts, 0);               // a text instruction is a text at each of its points
	EXPECT_GT(renderer.measures, 0);            // the painter asks the backend how large a text is
	EXPECT_GT(renderer.arcs, 0);
	EXPECT_GT(lookups, 0);

	const std::string hash = std::format("{:016x}", GISLibrary::JsonHash(renderer.text));
	if (hash != "6ae9eb1654a20698")
	{
		const auto dump = testpaths::tempDir() / "10100AA_X01NE.strokes.txt";
		std::ofstream(dump, std::ios::binary | std::ios::trunc) << renderer.text;
		ADD_FAILURE() << "recorded calls hash " << hash << ", strokes " << renderer.strokes << ", arcs " << renderer.arcs
			<< ", dashed " << renderer.dashed << ", symbols " << renderer.symbols << ", texts " << renderer.texts << ", measures " << renderer.measures
			<< ", kept paths " << renderer.retained << ", strokes of them " << renderer.retainedStrokes << ", line styles " << renderer.lineStyles << "; the text is in " << dump;
	}

	// Painted again, no curve is built again: its path is kept with it.
	const int kept = renderer.retained;
	const int strokesOfKept = renderer.retainedStrokes;
	for (auto& [key, instruction] : cell->pcManager->displayListSENC->displayInstructions)
	{
		instruction->Accept(painter);
	}
	EXPECT_EQ(renderer.retained, kept);
	EXPECT_EQ(renderer.retainedStrokes, 2 * strokesOfKept);
}
