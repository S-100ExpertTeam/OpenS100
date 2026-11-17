// The S-100 side of drawing (DisplayListPainter) against a renderer that only records what it
// is asked to draw.
#include <gtest/gtest.h>

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
				text += step.kind == s100::render::TransformStep::Kind::Rotate
					? std::format(" rotate {}", step.degrees)
					: std::format(" translate {} {}", step.x, step.y);
			}
			text += "\n";
		}

		void StrokePath(const s100::render::Path& path, const s100::render::ColorF& color,
			const s100::render::Stroke& stroke) override
		{
			strokes++;
			dashed += stroke.dashes.empty() ? 0 : 1;
			text += std::format("stroke width {} color {} {} {} {} dashes", stroke.width, color.r, color.g, color.b, color.a);
			for (const float dash : stroke.dashes)
			{
				text += std::format(" {}", dash);
			}
			for (const auto& figure : path.figures)
			{
				text += std::format(" | from {} {}", figure.start.x, figure.start.y);
				for (const auto& to : figure.lines)
				{
					text += std::format(" line {} {}", to.x, to.y);
				}
				text += figure.closed ? " closed" : " open";
			}
			for (const auto& arc : path.arcs)
			{
				arcs++;
				text += std::format(" | arc radius {} from {} to {}", arc.radius, arc.startDegrees, arc.endDegrees);
			}
			text += "\n";
		}
	};
}

// A sample cell with lights: its augmented rays and paths (sector limits and arcs) drawn on the
// recording renderer. No Direct2D and no window: this is what runs on every platform.
TEST(DisplayListPainter, AugmentedGeometryOfTheSampleCellIsStrokedAsRecorded)
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
	EXPECT_GE(renderer.strokes, rays);          // every ray is a stroke; a path has an arc a stroke
	EXPECT_GT(renderer.arcs, 0);
	EXPECT_GT(lookups, 0);

	const std::string hash = std::format("{:016x}", GISLibrary::JsonHash(renderer.text));
	if (hash != "d89733292a9a0986")
	{
		const auto dump = testpaths::tempDir() / "10100AA_X01NE.strokes.txt";
		std::ofstream(dump, std::ios::binary | std::ios::trunc) << renderer.text;
		ADD_FAILURE() << "recorded calls hash " << hash << ", strokes " << renderer.strokes << ", arcs " << renderer.arcs
			<< ", dashed " << renderer.dashed << "; the text is in " << dump;
	}
}
