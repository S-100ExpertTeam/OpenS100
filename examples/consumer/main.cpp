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
#include <GISLibrary/PCOutputSchemaManager.h>
#include <GISLibrary/S100Layer.h>
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
}
#endif

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "usage: opens100_consumer <feature catalogue xml> [portrayal catalogue xml] [S-101 cell]\n";
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
	if (argc >= 4)
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
	// Optional: portray the dataset with the rules of the portrayal catalogue. The result is
	// drawing instructions (S-100 Part 9): what is to be drawn, not a picture.
	if (argc >= 4)
	{
		PortrayalCatalogue pc;
		if (!pc.Open(std::string(argv[2])))
		{
			std::cerr << "cannot read " << argv[2] << "\n";
			return 1;
		}

		S100Layer layer(&fc, &pc);
		auto* cell = new S101Cell(&fc);   // the layer owns its dataset
		layer.m_spatialObject = cell;
		cell->SetLayer(&layer);
		if (!cell->Open(std::filesystem::path(argv[3]).wstring()))
		{
			std::cerr << "cannot read " << argv[3] << "\n";
			return 1;
		}

		layer.BuildPortrayalCatalogue();

		InstructionCounter count;
		if (cell->pcManager && cell->pcManager->displayListSENC)
		{
			for (auto& [key, instruction] : cell->pcManager->displayListSENC->displayInstructions)
			{
				instruction->Accept(count);
			}
		}
		std::cout << "drawing instructions: "
			<< (count.null + count.point + count.line + count.area + count.text + count.ray + count.path) << "\n"
			<< "  point " << count.point << ", line " << count.line << ", area " << count.area
			<< ", text " << count.text << ", augmented ray " << count.ray << ", augmented path " << count.path
			<< ", null " << count.null << "\n";
	}
#endif
	return 0;
}
