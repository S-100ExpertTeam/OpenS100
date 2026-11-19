// Smallest useful program on top of the installed opens100 package:
// read an S-100 feature catalogue and report what is in it.
#include <FeatureCatalog/FeatureCatalogue.h>
#include <FeatureCatalog/FeatureTypes.h>
#include <GeoMetryLibrary/MBR.h>
#include <PortrayalCatalogue/PortrayalCatalogue.h>
#ifdef OPENS100_CONSUMER_HAS_DATASET
#include <GISLibrary/S101Cell.h>
#endif

#ifdef OPENS100_CONSUMER_HAS_LUA
#include <LuaScriptingReference/lua_session.h>
#endif

#ifdef OPENS100_CONSUMER_HAS_PORTRAYAL
#include <GeoMetryLibrary/Scaler.h>
#include <charconv>
#include <cmath>
#include <cstdio>
#include <string>
#include <GISLibrary/DisplayListJson.h>
#include <GISLibrary/DisplayListPainter.h>
#include <GISLibrary/IRenderer.h>
#include <GISLibrary/PCOutputSchemaManager.h>
#include <GISLibrary/S100Layer.h>
#include <GISLibrary/S10XGML.h>
#include <GISLibrary/SENC_DisplayList.h>
#include <GISLibrary/SENC_Instruction.h>
#include <GISLibrary/SENC_InstructionVisitor.h>

namespace
{
	// What a renderer would be: it is handed every drawing instruction as what it is. This one
	// only counts them.
	struct InstructionCounter : SENC_InstructionVisitor
	{
		int null = 0, point = 0, line = 0, area = 0, text = 0, ray = 0, path = 0;

		void Visit(SENC_NullInstruction&) override { null++; }
		void Visit(SENC_PointInstruction&) override { point++; }
		void Visit(SENC_LineInstruction&) override { line++; }
		void Visit(SENC_AreaInstruction&) override { area++; }
		void Visit(SENC_TextInstruction&) override { text++; }
		void Visit(SENC_AugmentedRay&) override { ray++; }
		void Visit(SENC_AugmentedPath&) override { path++; }
	};

	// A render backend of the smallest kind: it is asked to stroke paths, place symbols and write texts, and writes down what
	// it was asked. A real one (Skia, Cairo, a canvas) draws them; the S-100 side of drawing,
	// where things go on the screen and how they are stroked, is DisplayListPainter and is the
	// same for all of them.
	class RecordingRenderer : public s100::render::IRenderer
	{
	public:
		std::string text;
		int strokes = 0;
		int symbols = 0;
		int texts = 0;

		void SetTransform(const s100::render::Transform& transform) override
		{
			text += "transform";
			for (const auto& step : transform)
			{
				if (step.kind == s100::render::TransformStep::Kind::Rotate)
				{
					text += " rotate";
					Number(step.degrees);
					text += " about";
					Number(step.x);
					Number(step.y);
				}
				else if (step.kind == s100::render::TransformStep::Kind::Scale)
				{
					// Not to a thousandth: the scale of a chart is a small number.
					char buffer[32];
					const int length = std::snprintf(buffer, sizeof(buffer), " scale %.6g", static_cast<double>(step.factor));
					text.append(buffer, static_cast<size_t>(length));
				}
				else
				{
					text += " translate";
					Number(step.x);
					Number(step.y);
				}
			}
			text += '\n';
		}

		void StrokePath(const s100::render::Path& path, const s100::render::ColorF& color,
			const s100::render::Stroke& stroke) override
		{
			strokes++;
			StrokeText(color, stroke);
			PathText(path);
			text += '\n';
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
			text += "path " + std::to_string(kept->number);
			PathText(path);
			text += '\n';
			return kept;
		}

		void StrokePath(const s100::render::RetainedPath& path, const s100::render::ColorF& color,
			const s100::render::Stroke& stroke) override
		{
			strokes++;
			StrokeText(color, stroke);
			text += " | path " + std::to_string(static_cast<const Kept&>(path).number) + "\n";
		}

		void StrokeLineStyle(const std::string& reference, const std::vector<s100::render::PointF>& points,
			const s100::render::ColorF* shortStretches) override
		{
			lineStyles++;
			text += "line style " + reference;
			if (shortStretches)
			{
				text += " color";
				Number(shortStretches->r);
				Number(shortStretches->g);
				Number(shortStretches->b);
				Number(shortStretches->a);
			}
			for (const auto& point : points)
			{
				Number(point.x);
				Number(point.y);
			}
			text += '\n';
		}

		int retained = 0;
		int lineStyles = 0;

	private:
		void StrokeText(const s100::render::ColorF& color, const s100::render::Stroke& stroke)
		{
			text += "stroke width";
			Number(stroke.width);
			text += " color";
			Number(color.r);
			Number(color.g);
			Number(color.b);
			Number(color.a);
			text += " dashes";
			for (const float dash : stroke.dashes)
			{
				Number(dash);
			}
		}

		void PathText(const s100::render::Path& path)
		{
			for (const auto& figure : path.figures)
			{
				text += " | from";
				Number(figure.start.x);
				Number(figure.start.y);
				for (const auto& to : figure.lines)
				{
					text += " line";
					Number(to.x);
					Number(to.y);
				}
				text += figure.closed ? " closed" : " open";
			}
			for (const auto& arc : path.arcs)
			{
				text += " | arc radius";
				Number(arc.radius);
				text += " from";
				Number(arc.startDegrees);
				text += " to";
				Number(arc.endDegrees);
			}
		}

	public:

		void DrawSymbol(const std::string& reference, s100::render::PointF at, float rotationDegrees, float scale) override
		{
			symbols++;
			text += "symbol " + reference + " at";
			Number(at.x);
			Number(at.y);
			text += " rotation";
			Number(rotationDegrees);
			text += " scale";
			Number(scale);
			text += '\n';
		}

		// A font with no glyphs: every byte of a text is half the font size wide.
		s100::render::SizeF MeasureText(const std::string& measured, const s100::render::TextFont& font) override
		{
			return { static_cast<float>(measured.size()) * font.size * 0.5f, font.size * 1.25f };
		}

		void FillText(const std::string& filled, s100::render::PointF topLeft, const s100::render::TextFont& font,
			const s100::render::ColorF& color) override
		{
			texts++;
			text += "text \"" + filled + "\" at";
			Number(topLeft.x);
			Number(topLeft.y);
			text += " size";
			Number(font.size);
			text += font.italic ? " italic" : " upright";
			text += " color";
			Number(color.r);
			Number(color.g);
			Number(color.b);
			Number(color.a);
			text += '\n';
		}

	private:
		// To a thousandth: the last bits of a coordinate differ between platforms (a point on
		// the edge of the dataset came out as 1e-11 on one and 0 on another).
		void Number(float value)
		{
			value = std::round(value * 1000.0f) / 1000.0f;
			if (value == 0)
			{
				value = 0;   // no "-0"
			}
			char buffer[32];
			const auto end = std::to_chars(buffer, buffer + sizeof(buffer), value).ptr;
			text += ' ';
			text.append(buffer, end);
		}
	};
}
#endif

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "usage: opens100_consumer <feature catalogue xml> [portrayal catalogue xml] [--json <directory>] [dataset ...]\n";
		return 2;
	}

	FeatureCatalogue fc;
	if (!fc.Read(std::string(argv[1])))
	{
		std::cerr << "cannot read " << argv[1] << "\n";
		return 1;
	}

	auto* featureTypes = fc.GetFeatureTypes();
	std::cout << "product:       " << fc.getProductId() << "\n"
		<< "version:       " << fc.GetVersionNumber() << "\n"
		<< "feature types: " << (featureTypes ? featureTypes->GetVecFeatureType().size() : 0) << "\n";

	// A second module, to prove more than one target links.
	MBR korea(124.0, 33.0, 132.0, 39.0);
	std::cout << "Busan inside the Korean box: " << std::boolalpha << korea.PtInMBR(129.04, 35.10) << "\n";

	// Optional: a portrayal catalogue, read as data (no resource store, so nothing is drawn).
	if (argc >= 3)
	{
		PortrayalCatalogue pc;
		if (!pc.Open(std::string(argv[2])))
		{
			std::cerr << "cannot read " << argv[2] << "\n";
			return 1;
		}

		auto* mainRule = pc.GetMainRuleFile();
		std::cout << "portrayal:     " << pc.GetProduct() << " " << pc.GetVersion() << "\n"
			<< "rule files:    " << pc.GetRules()->GetRuleFiles().size() << "\n"
			<< "symbols:       " << pc.GetSymbols()->GetSymbolsVector()->size() << "\n"
			<< "viewing groups: " << pc.GetViewingGroups()->GetViewingGroup()->size() << "\n"
			<< "main rule:     " << (mainRule ? mainRule->GetFileName() : "(none)") << "\n";
	}

#ifdef OPENS100_CONSUMER_HAS_LUA
	// The Lua engine S-101 portrayal rules run on. The rules format numbers with string.format;
	// how an exact half rounds is the engine's choice, so show it.
	{
		const auto script = std::filesystem::temp_directory_path() / "opens100_consumer.lua";
		std::ofstream(script)
			<< "function consumer_engine() return jit and jit.version or _VERSION end\n"
			<< "function consumer_format(f, v) return string.format(f, v) end\n";

		lua_session lua;
		lua.load_file(script.string());
		std::cout << "lua engine:    " << lua.call<std::string>("consumer_engine") << "\n"
			<< "%.0f of 0.5, 2.5: " << lua.call<std::string>("consumer_format", { "%.0f", 0.5 }) << ", "
			<< lua.call<std::string>("consumer_format", { "%.0f", 2.5 }) << "\n";
	}
#endif

#ifdef OPENS100_CONSUMER_HAS_DATASET
	// Optional: an S-101 dataset (ISO 8211), read against the feature catalogue above.
	if (argc >= 4 && std::filesystem::path(argv[3]).extension() == ".000")
	{
		const std::filesystem::path cellPath(argv[3]);
		S101Cell cell(&fc);
		if (!cell.Open(cellPath.wstring()))
		{
			std::cerr << "cannot read " << argv[3] << "\n";
			return 1;
		}

		std::cout << "dataset:       " << cellPath.filename().string() << "\n"
			<< "features:      " << cell.GetCount_FeatureTypeRecord() << "\n"
			<< "information:   " << cell.GetCount_InformationRecord() << "\n"
			<< "points:        " << cell.GetCount_PointRecord() << "\n"
			<< "curves:        " << cell.GetCount_CurveRecord() << "\n"
			<< "surfaces:      " << cell.GetCount_SurfaceRecord() << "\n";
	}
#endif

#ifdef OPENS100_CONSUMER_HAS_PORTRAYAL
	// Optional: portray every dataset given with the rules of the portrayal catalogue. The
	// result is drawing instructions (S-100 Part 9): what is to be drawn, not a picture.
	// An S-101 cell (ISO 8211) runs the Lua rules, a GML dataset the XSLT rules.
	if (argc >= 4)
	{
		PortrayalCatalogue pc;
		if (!pc.Open(std::string(argv[2])))
		{
			std::cerr << "cannot read " << argv[2] << "\n";
			return 1;
		}

		// --json <directory>: the display list of each dataset as a JSON file there.
		const char* jsonDirectory = nullptr;
		int firstDataset = 3;
		if (argc >= 5 && std::string(argv[3]) == "--json")
		{
			jsonDirectory = argv[4];
			firstDataset = 5;
		}

		for (int i = firstDataset; i < argc; i++)
		{
			const std::filesystem::path cellPath(argv[i]);
			S100Layer layer(&fc, &pc);
			S100SpatialObject* cell = nullptr;   // the layer owns its dataset
			if (cellPath.extension() == ".gml")
			{
				auto* gml = new S10XGML();
				gml->SetFC(&fc);
				cell = gml;
			}
			else
			{
				cell = new S101Cell(&fc);
			}
			layer.m_spatialObject = cell;
			cell->SetLayer(&layer);
			if (!cell->Open(cellPath.wstring()))
			{
				std::cerr << "cannot read " << argv[i] << "\n";
				return 1;
			}

			layer.BuildPortrayalCatalogue();
			if (!cell->pcManager || !cell->pcManager->displayListSENC)
			{
				std::cerr << "no display list for " << argv[i] << "\n";
				return 1;
			}
			auto& displayList = *cell->pcManager->displayListSENC;

			InstructionCounter count;
			for (auto& [key, instruction] : displayList.displayInstructions)
			{
				instruction->Accept(count);
			}

			// The same instructions as text. Its hash is the same on every platform, which is
			// how two builds are compared without drawing anything.
			const std::string json = GISLibrary::DisplayListToJson(displayList);
			if (jsonDirectory)
			{
				std::ofstream out(std::filesystem::path(jsonDirectory) / (cellPath.stem().string() + ".json"),
					std::ios::binary | std::ios::trunc);
				out << json;
			}

			std::cout << "portrayed:     " << cellPath.filename().string() << "\n"
				<< "drawing instructions: "
				<< (count.null + count.point + count.line + count.area + count.text + count.ray + count.path) << "\n"
				<< "  point " << count.point << ", line " << count.line << ", area " << count.area
				<< ", text " << count.text << ", augmented ray " << count.ray << ", augmented path " << count.path
				<< ", null " << count.null << "\n"
				<< "display list:  " << json.size() << " bytes, hash "
				<< std::hex << std::setw(16) << std::setfill('0') << GISLibrary::JsonHash(json)
				<< std::dec << std::setfill(' ') << "\n";

			// Drawing, as far as the library does it without a backend: the dataset fitted to a
			// 1600 x 1000 surface, the instructions painted on the recording renderer. Colours
			// are left as tokens (a renderer resolves them in its palette).
			Scaler scaler;
			scaler.SetScreen(0, 0, 1600, 1000);
			scaler.SetMap(layer.m_mbr);
			RecordingRenderer renderer;
			s100::render::DisplayListPainter painter(renderer, scaler, [](const std::wstring& token) {
				return s100::render::ColorF{ static_cast<float>(token.size()), token.empty() ? 0.0f : static_cast<float>(token[0]), 0.0f, 1.0f };
			});
			for (auto& [key, instruction] : displayList.displayInstructions)
			{
				instruction->Accept(painter);
			}
			std::cout << "painted:       " << renderer.symbols << " symbols, " << renderer.texts << " texts, " << renderer.strokes << " strokes, " << renderer.lineStyles << " line styles, hash "
				<< std::hex << std::setw(16) << std::setfill('0') << GISLibrary::JsonHash(renderer.text)
				<< std::dec << std::setfill(' ') << "\n";
			if (jsonDirectory)
			{
				std::ofstream out(std::filesystem::path(jsonDirectory) / (cellPath.stem().string() + ".painted.txt"),
					std::ios::binary | std::ios::trunc);
				out << renderer.text;
			}
		}
	}
#endif
	return 0;
}
